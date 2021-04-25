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
    GameObject::ToJson(js, backup.roots);
    Entity::SetNullify(true);
    GameObject::FromJson(js, scene.roots);
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
        Entity::SetNullify(true);
        GameObject::FromJson(js, roots);
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
        GameObject::ToJson(js, roots);
        
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
    roots.insert(root);
    return root;
}

GameObject *Scene::AddGameObject(GameObject *gameObject) {
    GameObject *root = gameObject->GetCopy();
    Transform *t = root->GetTransform();
    t->parent = nullptr;
    t->Propagate();
    roots.insert(root);
    return root;
}

void Scene::RemoveGameObject(GameObject *gameObject) {
    gameObject->GetTransform()->removed = true;
    gameObject->GetTransform()->SetLocalEnabled(false);
}

void Scene::RemoveComponent(Component *component) {
    component->removed = true;
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

    removes.clear();
    enables.clear();
    disables.clear();

    function<void(GameObject *, bool, bool)> recurse = [this, &recurse](GameObject *gameObject, bool enabled, bool removed) {
        Transform *transform = gameObject->GetTransform();
        for (Transform *t : transform->GetChildren()) {
            recurse(t->GetGameObject(), 
                enabled && t->IsLocalEnabled(), 
                removed || t->IsRemoved());
        }

        for (Component *component : gameObject->components) {
            if (!component->enabled && (enabled && component->IsLocalEnabled())) {
                enables.push_back(component);
                component->enabled = true;
            } else if (component->enabled && !(enabled && component->IsLocalEnabled())) {
                disables.push_back(component);
                component->enabled = false;
            }
            if (removed || component->IsRemoved()) {
                removes.push_back(component);
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
    for (Component *component : removes) {
        try {
            component->OnRemove();
        } catch(...) {}
    }
    for (Component *component : removes) {
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
    Track();
    Enable();
    Update();
    Render();
}

void Scene::PauseLoop() {
    Flags();

    Untrack();
    Track();
    Render();
}