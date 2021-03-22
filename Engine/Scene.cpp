#include <iostream>
#include <fstream>
#include <functional>
#include <algorithm>

#include <Project.hpp>
#include <Group.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>
#include <Component.hpp>
#include <Scene.hpp>
#include <Type.hpp>
#include <IBehavior.hpp>
#include <IRender.hpp>

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
        ifstream fs(path);
        if (fs.fail()) {
            cerr << '[' << __FUNCTION__ << ']' << " cannot open scene: " << name << '\n';
            return false;
        }

        // read json object
        json js;
        fs >> js;
        
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
        
        // read scene setting
        scene.setting = (SceneSetting *)SceneSetting::StaticType()->Instantiate();
        SceneSetting::StaticType()->Deserialize(js[SceneSetting::StaticType()->GetName()], scene.setting);
        
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
        SceneSetting::StaticType()->Serialize(js[SceneSetting::StaticType()->GetName()], scene.setting);

        // write entities
        json& entities = js["Entity"];
        function<void(GameObject *)> recurse = [&recurse, &entities](GameObject *gameObject) {
            Transform *transform = gameObject->GetTransform();
            size_t j = 0;
            for (size_t i=0; i<transform->children.size(); i++) {
                GameObject *child = transform->children[i]->GetGameObject();
                if (!child->IsRemoved()) {
                    recurse(child);
                    swap(transform->children[j], transform->children[i]);
                    j++;
                }
            }
            vector<Transform *> tempg(transform->children.begin() + j, transform->children.end());
            transform->children.resize(j);

            j = 0;
            for (size_t i=0; i<gameObject->components.size(); i++) {
                Component *component = gameObject->components[i];
                if (!component->IsRemoved()) {
                    Type *type = component->GetType();
                    type->Serialize(
                        entities[type->GetName()][to_string(reinterpret_cast<uint64_t>(component))], 
                        component);
                    swap(gameObject->components[j], gameObject->components[i]);
                    j++;
                }
            }
            vector<Component *> tempc(gameObject->components.begin() + j, gameObject->components.end());
            gameObject->components.resize(j);

            GameObject::StaticType()->Serialize(
                entities[GameObject::StaticType()->GetName()][to_string(reinterpret_cast<uint64_t>(gameObject))], 
                gameObject);
            transform->children.insert(transform->children.end(), tempg.begin(), tempg.end());
            gameObject->components.insert(gameObject->components.end(), tempc.begin(), tempc.end());
        };
        for (Group *group : scene.groups) {
            size_t j = 0;
            for (size_t i=0; i<group->gameObjects.size(); i++) {
                GameObject *gameObject = group->gameObjects[i];
                if (!gameObject->IsRemoved()) {
                    recurse(gameObject);
                    swap(group->gameObjects[j], group->gameObjects[i]);
                    j++;
                }
            }
            vector<GameObject *> temp(group->gameObjects.begin() + j, group->gameObjects.end());
            group->gameObjects.resize(j);

            Group::StaticType()->Serialize(
                entities[Group::StaticType()->GetName()][to_string(reinterpret_cast<uint64_t>(group))], 
                group);
            group->gameObjects.insert(group->gameObjects.end(), temp.begin(), temp.end());
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
    groups.push_back(group);
    return group;
}

void Scene::RemoveGroup(Group *group) {
    delete group;
    groups.erase(find(groups.begin(), groups.end(), group));
}

GameObject *Scene::GetGameObject(const string &name) {
    for (Group *group : groups) {
        if (GameObject *gameObject = group->GetGameObject(name)) {
            return gameObject;
        }
    }
    return nullptr;
}

void Scene::Start() {
    for (Group *group : groups) {
        for (Component *ibehavior : group->ibehaviors) {
            try {
                ((IBehavior *)ibehavior)->Start();
            } catch(...) {}
        }
    } 
}

void Scene::Update() {
    for (Group *group : groups) {
        for (Component *ibehavior : group->ibehaviors) {
            try {
                ((IBehavior *)ibehavior)->Update();
            } catch(...) {}
        }
    }
}

void Scene::Render() {
    for (Group *group : groups) {
        for (Component *irender : group->irenders) {
            try {
                ((IRender *)irender)->Render();
            } catch(...) {}
        }
    }
}