#include <iostream>
#include <fstream>
#include <functional>
#include <filesystem>
#include <algorithm>

#include <Project.hpp>
#include <Group.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>
#include <Scene.hpp>
#include <Script.hpp>

using json = nlohmann::json;
using namespace std;
using namespace Engine;

bool Scene::Load(const string &path) {
    Scene &scene = Scene::GetInstance();

    // close project
    Scene::Close();

    try {
        // get scene file path
        scene.path = path;
        scene.name = filesystem::path(scene.path).stem().string();
        
        // open json file
        ifstream fs(Project::GetInstance().GetDirectoy() + "/" + scene.path);
        if (fs.fail()) {
            cerr << '[' << __FUNCTION__ << ']' << " cannot open scene file: " << scene.path << '\n';
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
            for (json::iterator j = i.value().begin(); j != i.value().end(); j++) {
                type->Deserialize(j.value(), Entity::temp.at(stoll(j.key())));
            }
        }
        
        // read extras
        scene.groups = js["groups"].get<unordered_set<Group *>>();
        scene.scripts = js["scripts"].get<unordered_set<Script *>>();
        scene.renderers = js["renderers"].get<multiset<Renderer *, RendererComparer>>();
        scene.drawers = js["drawers"].get<multiset<Drawer *, DrawerComparer>>();

        // read scene setting
        scene.setting = (SceneSetting *)SceneSetting::StaticType()->Instantiate();
        SceneSetting::StaticType()->Deserialize(js["setting"], scene.setting);
        
        Entity::temp.clear();
    } catch(...) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot read scene: " << scene.name << '\n';
        Scene::Close();
        return false;
    }
    
    scene.loaded = true;
    cerr << '[' << __FUNCTION__ << ']' << " read scene: " << scene.name << " done.\n";
    return true;
}

bool Scene::Save() {
    Scene &scene = Scene::GetInstance();
    
    // open json file
    ofstream fs(Project::GetInstance().GetDirectoy() + "/" + scene.path);
    if (fs.fail()) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot open scene: " << scene.name << '\n';
        return false;
    }
    
    try {
        json js;

        // write scene setting
        SceneSetting::StaticType()->Serialize(js["setting"], scene.setting);

        // write extras
        js["groups"] = scene.groups;
        js["scripts"] = scene.scripts;
        js["renderers"] = scene.renderers;
        js["drawers"] = scene.drawers;

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
        for (Group *group : scene.groups) {
            for (GameObject *gameObject : group->gameObjects) {
                recurse(gameObject);
            }

            Group::StaticType()->Serialize(
                entities[Group::StaticType()->GetName()][to_string(reinterpret_cast<uint64_t>(group))], 
                group);
        }
        
        fs << js;
    } catch(...) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot save scene: " << scene.name << '\n';
        return false;
    }

    cerr << '[' << __FUNCTION__ << ']' << " save scene: " << scene.name << "done.\n";
    return true;
}

void Scene::Close() {
    Scene &scene = Scene::GetInstance();
    scene.name.clear();
    scene.path.clear();
    if (scene.setting) {
        delete scene.setting;
    }
    vector<Group *> temp(scene.groups.begin(), scene.groups.end());
    for (Group *group : temp) {
        delete group;
    }
    scene.groups.clear();
    scene.garbages.clear();

    scene.loaded = false;
}

Group *Scene::AddGroup() {
    Group *group = new Group();
    return group;
}

void Scene::RemoveGroup(Group *group) {
    garbages.insert(group);
}

GameObject *Scene::FindGameObject(const string &name) {
    for (Group *group : groups) {
        if (GameObject *gameObject = group->FindGameObject(name)) {
            return gameObject;
        }
    }
    return nullptr;
}

void Scene::Start() {
    for (Script *script : scripts) {
        try {
            if (script->IsEnabled()) {
                script->Start();
            }
        } catch(...) {}
    }
}

void Scene::Refresh() {
    for (Group *group : groups) {
        if (group->dirty) {
            group->Refresh();
        }
    }
}

void Scene::Update() {
    for (Script *script : scripts) {
        try {
            if (script->IsEnabled()) {
                script->Update();
            }
        } catch(...) {}
    }
}

void Scene::Render() {
    for (Renderer *renderer : renderers) {
        try {
            if (renderer->IsEnabled()) {
                renderer->Render();
            }
        } catch(...) {}
    }
}

void Scene::Delete() {
    for (Group *garbage : garbages) {
        delete garbage;
    }
    garbages.clear();
}