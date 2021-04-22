#include <GL/glew.h>

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

using json = nlohmann::json;
using namespace std;
using namespace Engine;

Scene Scene::backup;

void Scene::SaveBackup() {

}

void Scene::LoadBackup() {

}

bool Scene::Load(string path) {
    // close project
    Close();

    try {
        // get scene file path
        this->path = path;
        name = filesystem::u8path(this->path).stem().string();
        
        // open json file
        ifstream fs(filesystem::u8path(Project::GetInstance().GetDirectoy() + "/" + this->path));
        if (fs.fail()) {
            cerr << '[' << __FUNCTION__ << ']' << " cannot open scene file: " << this->path << '\n';
            return false;
        }

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
            if (type == Drawer::StaticType()) {
                // read drawers and batches
                for (json::iterator j = i.value().begin(); j != i.value().end(); j++) {
                    Drawer *drawer = (Drawer *)Entity::temp.at(stoll(j.key()));
                    type->Deserialize(j.value(), (Entity *)drawer);
                    batches[BatchKey{drawer->GetMesh().get(), drawer->GetMaterial().get()}].GetDrawers().insert(drawer);
                }
            } else {
                for (json::iterator j = i.value().begin(); j != i.value().end(); j++) {
                    type->Deserialize(j.value(), Entity::temp.at(stoll(j.key())));
                }
            }
        }
        
        // read roots 
        roots = js["roots"].get<unordered_set<GameObject *>>();

        // read scene setting
        setting = (SceneSetting *)SceneSetting::StaticType()->Instantiate();
        SceneSetting::StaticType()->Deserialize(js["setting"], setting);

        Entity::temp.clear();
    } catch(...) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot read scene: " << name << '\n';
        Close();
        return false;
    }
    
    loaded = true;
    cerr << '[' << __FUNCTION__ << ']' << " read scene: " << name << " done.\n";
    return true;
}

bool Scene::Save() {
    try {
        json js;

        // write scene setting
        SceneSetting::StaticType()->Serialize(js["setting"], setting);

        // write roots
        js["roots"] = roots;

        // write entities
        json& entities = js["entities"];
        function<void(GameObject *)> recurse = [&recurse, &entities](GameObject *gameObject) {
            Transform *transform = gameObject->GetTransform();
            for (Transform *t : transform->children) {
                recurse(t->GetGameObject());
            }

            for (Component *component : gameObject->components) {
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
            cerr << '[' << __FUNCTION__ << ']' << " cannot open scene: " << name << '\n';
            return false;
        }
        fs << js;
    } catch(...) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot save scene: " << name << '\n';
        Close();
        return false;
    }

    cerr << '[' << __FUNCTION__ << ']' << " save scene: " << name << " done.\n";
    return true;
}

void Scene::Close() {
    name.clear();
    path.clear();
    if (setting) {
        delete setting;
        setting = nullptr;
    }
    for (GameObject *root : roots) {
        delete root;
    }
    roots.clear();

    loaded = false;
    cerr << '[' << __FUNCTION__ << ']' << " close scene done\n";
}

GameObject *Scene::AddGameObject() {
    GameObject *root = new GameObject();
    Transform *transform = root->AddComponent<Transform>();
    roots.insert(root);
    return root;
}

GameObject *Scene::AddGameObject(GameObject *gameObject) {
    
}

void Scene::RemoveGameObject(GameObject *gameObject) {
    gameObject->GetTransform()->SetFlags(Component::Remove);
    gameObject->GetTransform()->SetLocalEnabled(false);
}

void Scene::RemoveComponent(Component *component) {
    component->SetFlags(Component::Remove);
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
}

void Scene::Destroy() {
    for (Component *component : removes) {
        try {
            component->OnDestroy();
            delete component;
        } catch(...) {}
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
    for (Renderer *renderer : renderers) {
        try {
            renderer->Render();
        } catch(...) {}
    }
}

void Scene::PauseLoop() {
    enables.clear();
    disables.clear();

    function<void(GameObject *, bool)> recurse = [this, &recurse](GameObject *gameObject, bool enabled) {
        Transform *transform = gameObject->GetTransform();
        for (Transform *t : transform->GetChildren()) {
            recurse(t->GetGameObject(), enabled && t->IsLocalEnabled());    
        }

        for (Component *component : gameObject->components) {
            if (!component->enabled && (enabled && component->IsLocalEnabled())) {
                enables.push_back(component);
                component->enabled = true;
            }
            if (component->enabled && !(enabled && component->IsLocalEnabled())) {
                disables.push_back(component);
                component->enabled = false;
            }
        }
    };

    for (GameObject *root : roots) {
        recurse(root, true);
    }

    Disable();
    Enable();
    Render();
}

void Scene::PlayLoop() {
    adds.clear();
    removes.clear();
    enables.clear();
    disables.clear();

    function<void(GameObject *, bool)> recurse = [this, &recurse](GameObject *gameObject, bool enabled) {
        Transform *transform = gameObject->GetTransform();
        for (Transform *t : transform->GetChildren()) {
            recurse(t->GetGameObject(), enabled && t->IsLocalEnabled());    
        }

        for (Component *component : gameObject->components) {
            if (!component->enabled && (enabled && component->IsLocalEnabled())) {
                enables.push_back(component);
                component->enabled = true;
            }
            if (component->enabled && !(enabled && component->IsLocalEnabled())) {
                disables.push_back(component);
                component->enabled = false;
            }
            if (component->flags & Component::Add) {
                adds.push_back(component);
            }
            if (component->flags & Component::Remove) {
                removes.push_back(component);
            }
        }
    };

    for (GameObject *root : roots) {
        recurse(root, true);
    }

    Disable();
    Remove();
    Destroy();
    Add();
    Enable();
    Update();
    Render();
}