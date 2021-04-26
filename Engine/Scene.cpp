#include <iostream>
#include <fstream>
#include <functional>
#include <filesystem>
#include <algorithm>

#include <Project.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>
#include <Scene.hpp>
#include <Script.hpp>
#include <Graphics/Drawer.hpp>
#include <Graphics/Renderer.hpp>

using json = nlohmann::json;
using namespace std;
using namespace Engine;

void Scene::ToBackup() {
    Scene &scene = GetInstance();
    Scene &backup = GetBackup();
    backup.CloseImmediate();
    backup = scene;
    scene = Scene();
}

void Scene::FromBackup() {
    Scene &scene = GetInstance();
    Scene &backup = GetBackup();
    scene.CloseImmediate();
    scene = backup;
    backup = Scene();
}

void Scene::LoadBackup() {
    Scene &scene = GetInstance();
    Scene &backup = GetBackup();
    scene.name = backup.name;
    scene.path = backup.path;
    scene.loaded = true;
    json js;
    GameObject::ToJson(js, backup.roots, false);
    GameObject::FromJson(js, scene.roots, true);
}

void Scene::Load(const string &path) {
    flags |= LOAD;
    this->loadPath = path;
}

void Scene::Save() {
    flags |= SAVE;
}

void Scene::Close() {
    flags |= CLOSE;
}

bool Scene::LoadImmediate(const string &path) {
    try {
        // open json file
        ifstream fs(filesystem::u8path(Project::GetInstance().GetDirectoy() + "/" + path));
        if (fs.fail()) {
            cerr << '[' << __FUNCTION__ << ']' << " cannot open scene file: " << path << '\n';
            return false;
        }

        // move to backup in order to maintain duplicate resource and handle failure cases
        ToBackup();

        // get scene file path
        this->path = path;
        name = filesystem::u8path(this->path).stem().string();

        // read json object
        json js;
        fs >> js;
        
        // read roots
        GameObject::FromJson(js, roots, true);
    } catch(...) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot read scene: " << path << '\n';
        FromBackup();
        return false;
    }
    
    GetBackup().CloseImmediate();

    loaded = true;
    cerr << '[' << __FUNCTION__ << ']' << " read scene: " << path << " done.\n";
    return true;
}

bool Scene::SaveImmediate() {
    try {
        json js;

        // write roots
        GameObject::ToJson(js, roots, false);
        
        // open json file
        ofstream fs(filesystem::u8path(Project::GetInstance().GetDirectoy() + "/" + path));
        if (fs.fail()) {
            cerr << '[' << __FUNCTION__ << ']' << " cannot open scene: " << path << '\n';
            return false;
        }
        fs << js;
    } catch(...) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot save scene: " << path << '\n';
        return false;
    }

    cerr << '[' << __FUNCTION__ << ']' << " save scene: " << path << " done.\n";
    return true;
}

void Scene::CloseImmediate() {
    function<void(GameObject *)> recurse = [this, &recurse](GameObject *gameObject) {
        Transform *transform = gameObject->GetTransform();
        for (Transform *t : transform->GetChildren()) {
            recurse(t->GetGameObject());
        }

        auto components = gameObject->components;
        for (Component *component : components) {
            delete component;
        }
    };
    for (GameObject *root : roots) {
        recurse(root);
    }

    for (auto &order : batches) {
        for (auto &batch : order.second) {
            delete batch.second;
        }
    }

    *this = Scene();

    cerr << '[' << __FUNCTION__ << ']' << " close scene done.\n";
}

GameObject *Scene::AddGameObject() {
    GameObject *root = new GameObject();
    root->AddComponent<Transform>();
    roots.push_back(root);
    return root;
}

GameObject *Scene::AddGameObject(GameObject *gameObject) {
    GameObject *root = gameObject->GetCopy();
    Transform *t = root->GetTransform();
    t->parent = nullptr;
    t->Propagate();
    roots.push_back(root);
    return root;
}

void Scene::RemoveGameObject(GameObject *gameObject) {
    gameObject->GetTransform()->flags |= Component::REMOVED;
    gameObject->GetTransform()->SetLocalEnabled(false);
}

void Scene::RemoveComponent(Component *component) {
    component->flags |= Component::REMOVED;
    component->SetLocalEnabled(false);
}

GameObject *Scene::FindGameObject(const string &name) {
    for (GameObject *root : roots) {
        if (GameObject *gameObject = root->FindGameObject(name)) {
            return gameObject;
        }
    }
    return nullptr;
}

void Scene::Flags() {
    if (flags & CLOSE) {
        CloseImmediate();
    }
    if (flags & LOAD) {
        LoadImmediate(loadPath);
    }
    if (flags & SAVE) {
        SaveImmediate();
    }
    flags = 0U; 

    removeComps.clear();
    removeTrans.clear();
    enables.clear();
    disables.clear();

    function<void(GameObject *, bool, bool)> recurse = [this, &recurse](GameObject *gameObject, bool enabled, bool removed) {
        Transform *transform = gameObject->GetTransform();
        for (Transform *t : transform->GetChildren()) {
            recurse(t->GetGameObject(), 
                enabled && t->IsLocalEnabled(), 
                removed || t->IsRemoved());
        }
        if (removed || transform->IsRemoved()) {
            removeTrans.push_back(transform);
        }

        for (Component *component : gameObject->components) {
            if (!(component->flags & Component::ENABLED) && (enabled && component->IsLocalEnabled())) {
                enables.push_back(component);
                component->flags |= Component::ENABLED;
            } else if ((component->flags & Component::ENABLED) && !(enabled && component->IsLocalEnabled())) {
                disables.push_back(component);
                component->flags |= ~Component::ENABLED;
            }
            if (removed || component->IsRemoved()) {
                removeComps.push_back(component);
            }
        }
    };

    for (GameObject *root : roots) {
        recurse(root, root->IsLocalEnabled(), root->IsRemoved());
    }
}

void Scene::Track() {
    for (Component *component : enables) {
        component->OnTrack();
    }
}

void Scene::Untrack() {
    for (Component *component : disables) {
        component->OnUntrack();
    }
}

void Scene::Enable() {
    for (Component *component : enables) {
        try {
            component->OnEnable();
        } catch(...) {}
    }
}

void Scene::Disable() {
    for (Component *component : disables) {
        try {
            component->OnDisable();
        } catch(...) {}
    }
}

void Scene::Remove() {
    for (Component *component : removeComps) {
        try {
            component->OnDestroy();
        } catch(...) {}
    }
    sort(removeTrans.begin(), removeTrans.end(), [](const Transform *l, const Transform *r)->bool {
        return l->GetParent() < r->GetParent();
    });
    sort(removeComps.begin(), removeComps.end(), [](const Component *l, const Component *r)->bool {
        return l->GetGameObject() < r->GetGameObject();
    });
    for (size_t i = 0; i < removeTrans.size(); i++) {
        size_t j = i;
        do {
            removeTrans[j]->flags |= Component::DESTROYED;
            j++;
        } while (j < removeTrans.size() && removeTrans[j - 1]->GetParent() == removeTrans[j]->GetParent());
        Transform *transform = removeTrans[i]->GetParent();
        size_t l = 0;
        if (transform) {
            for (size_t k = 0; k < transform->children.size(); k++) {
                if (transform->children[k]->flags & Component::DESTROYED) {
                    continue;
                }
                transform->children[l] = transform->children[k];
                l++;
            }
            transform->children.resize(l);
        } else {
            for (size_t k = 0; k < roots.size(); k++) {
                if (roots[k]->GetTransform()->flags & Component::DESTROYED) {
                    continue;
                }
                roots[l] = roots[k];
                l++;
            }
            roots.resize(l);
        }
        i = j - 1;
    }
    for (size_t i = 0; i < removeComps.size(); i++) {
        size_t j = i;
        do {
            removeComps[j]->flags |= Component::DESTROYED;
            j++;
        } while (j < removeComps.size() && removeComps[j - 1]->GetGameObject() == removeComps[j]->GetGameObject());
        GameObject *gameObject = removeComps[i]->GetGameObject();
        size_t l = 0;
        for (size_t k = 0; k < gameObject->components.size(); k++) {
            if (gameObject->components[k]->flags & Component::DESTROYED) {
                continue;
            }
            gameObject->components[l] = gameObject->components[k];
            l++;
        }
        gameObject->components.resize(l);
        i = j - 1;
    }
}

void Scene::Destroy() {
    for (Component *component : removeComps) {
        delete component;
    }
}

void Scene::Update() {
    for (Script *script : scripts) {
        try {
            if (script->start) {
                script->Start();
                script->start = false;
            } else {
                script->Update();
            }
        } catch(...) {}
    }
}

void Scene::Render() {
    for (auto &order : renderers) {
        for (Renderer *renderer : order.second) {
            try {
                renderer->Render();
            } catch(...) {}
        }
    }
}

void Scene::Loop() {
    Flags();

    Untrack();
    Disable();
    Remove();
    Destroy();
    Track();
    Enable();
    Update();
    Render();
}

void Scene::PauseLoop() {
    Flags();

    Untrack();
    Remove();
    Track();
    Render();
}