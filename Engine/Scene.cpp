#include <iostream>
#include <fstream>

#include <Project.hpp>
#include <Group.hpp>
#include <GameObject.hpp>
#include <Component.hpp>
#include <Scene.hpp>
#include <Type.hpp>

using json = nlohmann::json;
using namespace std;
using namespace Engine;

bool Scene::Load(const string &name) {
    Scene &scene = Scene::GetInstance();
    // close project
    Scene::Close();

    try {
        // get scene file path
        const string &path = Project::GetInstance().GetScene(name);

        // open json file
        ifstream fs(name);
        if (fs.fail()) {
            cerr << '[' << __FUNCTION__ << ']' << " cannot open scene: " << name << '\n';
            return false;
        }

        // read json object
        json js;
        fs >> js;
        
        // read scene setting
        SceneSetting::type->Deserialize(js[SceneSetting::type->GetName()], scene.setting);
        
        // read entities
        json &entities = js["Entity"];
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
        Entity::temp.clear();
    } catch(...) {
        Scene::Close();
        cerr << '[' << __FUNCTION__ << ']' << " cannot read scene: " << name << '\n';
        return false;
    }
    
    cerr << '[' << __FUNCTION__ << ']' << " read scene: " << name << " done.\n";
    return true;
}

bool Scene::Save() {
    Scene &scene = Scene::GetInstance();
    
    // open json file
    ofstream fs(scene.name);
    if (fs.fail()) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot open scene: " << scene.name << '\n';
        return false;
    }
    
    try {
        json js;

        // write scene setting
        SceneSetting::type->Serialize(js[SceneSetting::type->GetName()], scene.setting);

        // write entities
        json& entities = js["Entity"];
        for (Group *group : scene.groups) {
            Group::type->Serialize(
                entities[Group::type->GetName()][to_string(reinterpret_cast<uint64_t>(group))], 
                group);
            for (auto i = group->gameObjects.begin(); i != group->gameObjects.end(); ) {
                GameObject *gameObject = i->second;
                if (gameObject->IsRemoved()) {
                    delete gameObject;
                    i = group->gameObjects.erase(i);
                } else {
                    GameObject::type->Serialize(
                        entities[GameObject::type->GetName()][to_string(reinterpret_cast<uint64_t>(gameObject))], 
                        gameObject);
                    for (auto j = gameObject->components.begin(); j != gameObject->components.end(); ) {
                        Component *component = *j;
                        if (component->IsRemoved()) {
                            delete component;
                            j = gameObject->components.erase(j);
                        } else {
                            Type *type = component->GetType();
                            type->Serialize(
                                entities[type->GetName()][to_string(reinterpret_cast<uint64_t>(component))], 
                                component);
                            j++;
                        }
                    }
                    i++;
                }
            }
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
    delete scene.setting;
    for (Group *group : scene.groups) {
        delete group;
    }
    scene.groups.clear();
}

Group *Scene::AddGroup() {
    Group *group = new Group();
    groups.insert(group);
    return group;
}

void Scene::RemoveGroup(Group *group) {
    delete group;
    groups.erase(group);
}

GameObject *Scene::GetGameObject(const string &name) {
    for (Group *group : groups) {
        if (GameObject *gameObject = group->GetGameObject(name)) {
            return gameObject;
        }
    }
    return nullptr;
}