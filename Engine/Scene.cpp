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

bool Scene::LoadImmediate(const string &path, bool useBackup) {
    try {
        // open json file
        ifstream fs(filesystem::u8path(Project::GetInstance().GetDirectoy() + "/" + path));
        if (fs.fail()) {
            cerr << '[' << __FUNCTION__ << ']' << " cannot open scene file: " << path << '\n';
            return false;
        }

        // move to backup in order to maintain duplicate resource and handle failure cases
        if (useBackup) { ToBackup(); }

        // get scene file path
        this->path = path;
        name = filesystem::u8path(this->path).stem().string();

        // read json object
        json js;
        fs >> js;
        
        // read entities
        json &entities = js["entities"];
        for (json::iterator i = entities.begin(); i != entities.end(); i++) {
            const Type *type = Type::GetType(i.key());
            for (json::iterator j = i.value().begin(); j != i.value().end(); j++) {
                Entity *entity = type->Instantiate();
                Entity::temp.insert({stoll(j.key()), entity});
            }
        }

        for (json::iterator i = entities.begin(); i != entities.end(); i++) {
            const Type *type = Type::GetType(i.key());
            for (json::iterator j = i.value().begin(); j != i.value().end(); j++) {
                type->Deserialize(j.value(), Entity::temp.at(stoll(j.key())));
            }
        }
        
        // read roots 
        roots = js["roots"].get<unordered_set<GameObject *>>();

        // read scene setting
        setting = (SceneSetting *)SceneSetting::StaticType()->Instantiate();
        SceneSetting::StaticType()->Deserialize(js["setting"], setting);

        Entity::temp.clear();

        // process components
        function<void(GameObject *, bool)> recurse = [this, &recurse](GameObject *gameObject, bool enabled) {
            Transform *transform = gameObject->GetTransform();
            for (Transform *t : transform->GetChildren()) {
                recurse(t->GetGameObject(), enabled && t->IsLocalEnabled());
            }

            for (Component *component : gameObject->components) {
                adds.push_back(component);
                if (enabled && component->IsLocalEnabled()) {
                    enables.push_back(component);
                }
            }
        };
        for (GameObject *root : roots) {
            recurse(root, root->IsLocalEnabled());
        }
        Add();
        Enable();
        adds.clear();
        enables.clear();

    } catch(...) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot read scene: " << path << '\n';
        if (useBackup) { FromBackup(); }
        return false;
    }
    
    if (useBackup) { GetBackup().CloseImmediate(); }

    loaded = true;
    cerr << '[' << __FUNCTION__ << ']' << " read scene: " << path << " done.\n";
    return true;
}

bool Scene::SaveImmediate() {
    try {
        json js;

        // write scene setting
        SceneSetting::StaticType()->Serialize(js["setting"], setting);

        // write roots
        js["roots"] = roots;

        // write entities
        json& entities = js["entities"];
        function<void(GameObject *)> recurse = [&recurse, &entities](GameObject *gameObject) {
            // ignore removed GameObjects
            if (gameObject->IsRemoved()) {
                return;
            }

            Transform *transform = gameObject->GetTransform();
            for (Transform *t : transform->children) {
                recurse(t->GetGameObject());
            }

            for (Component *component : gameObject->components) {
                // ignore removed Components
                if (component->IsRemoved()) {
                    continue;
                }

                Type *type = component->GetType();
                    type->Serialize(
                        entities[type->GetName()][to_string(reinterpret_cast<uint64_t>(component))],
                        component); 
            }

            GameObject::StaticType()->Serialize(
                entities[GameObject::StaticType()->GetName()][to_string(reinterpret_cast<uint64_t>(gameObject))], 
                gameObject);
        };
        for (GameObject *root : roots) {
            recurse(root);
            
            GameObject::StaticType()->Serialize(
                entities[GameObject::StaticType()->GetName()][to_string(reinterpret_cast<uint64_t>(root))], 
                root);
        }
        
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
    if (setting) {
        delete setting;
    }
    
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

    cerr << '[' << __FUNCTION__ << ']' << " close scene done\n";
}

GameObject *Scene::AddGameObject() {
    GameObject *root = new GameObject();
    Transform *transform = root->AddComponent<Transform>();
    roots.insert(root);
    return root;
}

GameObject *Scene::AddGameObject(GameObject *gameObject) {
    return nullptr;
}

void Scene::RemoveGameObject(GameObject *gameObject) {
    gameObject->GetTransform()->SetFlags(Component::REMOVE);
    gameObject->GetTransform()->SetLocalEnabled(false);
}

void Scene::RemoveComponent(Component *component) {
    component->SetFlags(Component::REMOVE);
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

    adds.clear();
    removes.clear();
    enables.clear();
    disables.clear();

    function<void(GameObject *, bool, bool, bool)> recurse = [this, &recurse](GameObject *gameObject, bool enabled, bool added, bool removed) {
        Transform *transform = gameObject->GetTransform();
        for (Transform *t : transform->GetChildren()) {
            recurse(t->GetGameObject(), 
                enabled && t->IsLocalEnabled(), 
                added || (t->flags & Component::ADD), 
                removed || (t->flags & Component::REMOVE));
        }

        for (Component *component : gameObject->components) {
            if (!component->enabled && (enabled && component->IsLocalEnabled())) {
                enables.push_back(component);
                component->enabled = true;
            } else if (component->enabled && !(enabled && component->IsLocalEnabled())) {
                disables.push_back(component);
                component->enabled = false;
            }
            if (added || (component->flags & Component::ADD)) {
                adds.push_back(component);
            }
            if (removed || (component->flags & Component::REMOVE)) {
                removes.push_back(component);
            }
        }
    };

    for (GameObject *root : roots) {
        recurse(root, root->IsLocalEnabled(), root->GetTransform()->flags & Component::ADD, root->GetTransform()->flags & Component::REMOVE);
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

void Scene::Add() {
    for (Component *component : adds) {
        try {
            component->OnAdd();
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
            script->Update();
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

    Disable();
    Remove();
    Add();
    Enable();
    Update();
    Render();
}