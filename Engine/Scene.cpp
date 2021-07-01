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
#include <Prefab.hpp>
#include <Graphics/Drawer.hpp>
#include <Graphics/Renderer.hpp>

using json = nlohmann::json;
using namespace std;

void Scene::ToBackup() {
    Scene &scene = GetInstance();
    Scene &backup = GetBackup();
    backup.Close();
    backup = scene;
    scene = Scene();
}

void Scene::FromBackup() {
    Scene &scene = GetInstance();
    Scene &backup = GetBackup();
    scene.Close();
    scene = backup;
    backup = Scene();
}

void Scene::Load(const string &path) {
    flags |= LOAD;
    this->loadPath = path;
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
        FromJson(js);
    } catch(...) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot read scene: " << path << '\n';
        FromBackup();
        return false;
    }
    
    GetBackup().Close();

    loaded = true;
    cerr << '[' << __FUNCTION__ << ']' << " read scene: " << path << " done.\n";
    return true;
}

bool Scene::Save(const json &js) {
    // write to scene file
    ofstream fs(filesystem::u8path(Project::GetInstance().GetDirectoy() + "/" + path));
    if (fs.fail()) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot open scene: " << path << '\n';
        return false;
    }
    fs << js;

    cerr << '[' << __FUNCTION__ << ']' << " save scene: " << path << " done.\n";
    return true;
}

void Scene::Close() {
    root->Destroy();

    for (auto &order : batches) {
        for (auto &batch : order.second) {
            delete batch.second;
        }
    }

    *this = Scene();
}

void Scene::Flags() {
    if (flags & LOAD) {
        LoadImmediate(loadPath);
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
                component->flags &= ~Component::ENABLED;
            }
            if (removed || component->IsRemoved()) {
                removeComps.push_back(component);
            }
        }
    };

    recurse(root, root->IsLocalEnabled(), false); // root should not be removed
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
        for (size_t k = 0; k < transform->children.size(); k++) {
            if (transform->children[k]->flags & Component::DESTROYED) {
                continue;
            }
            transform->children[l] = transform->children[k];
            l++;
        }
        transform->children.resize(l);
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