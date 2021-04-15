#include <GL/glew.h>

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
            for (json::iterator j = i.value().begin(); j != i.value().end(); j++) {
                type->Deserialize(j.value(), Entity::temp.at(stoll(j.key())));
            }
        }
        
        // read extras
        groups = js["groups"].get<unordered_set<Group *>>();
        scripts = js["scripts"].get<unordered_set<Script *>>();
        renderers = js["renderers"].get<multiset<Renderer *, RendererComparer>>();
        drawers = js["drawers"].get<multiset<Drawer *, DrawerComparer>>();

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

        // write extras
        js["groups"] = groups;
        js["scripts"] = scripts;
        js["renderers"] = renderers;
        js["drawers"] = drawers;

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
        for (Group *group : groups) {
            for (GameObject *gameObject : group->gameObjects) {
                recurse(gameObject);
            }

            Group::StaticType()->Serialize(
                entities[Group::StaticType()->GetName()][to_string(reinterpret_cast<uint64_t>(group))], 
                group);
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
    for (Group *group : groups) {
        delete group;
    }
    groups.clear();
    for (Group *garbage : garbages) {
        delete garbage;
    }
    garbages.clear();

    loaded = false;
    cerr << '[' << __FUNCTION__ << ']' << " close scene done\n";
}

Group *Scene::AddGroup() {
    Group *group = new Group();
    groups.insert(group);
    return group;
}

unordered_set<Group *>::iterator Scene::RemoveGroup(unordered_set<Group *>::iterator it) {
    if (it == groups.end()) {
        return groups.end();
    }
    
    Group *group = *it;
    for (auto it = scripts.begin(); it != scripts.end(); ) {
        Script *script = *it;
        if (script->GetGroup() == group) {
            it = scripts.erase(it);
        } else {
            it++;
        }
    }
    for (auto it = drawers.begin(); it != drawers.end(); ) {
        Drawer *drawer = *it;
        if (drawer->GetGroup() == group) {
            it = drawers.erase(it);
        } else {
            it++;
        }
    }
    for (auto it = renderers.begin(); it != renderers.end(); ) {
        Renderer *renderer = *it;
        if (renderer->GetGroup() == group) {
            it = renderers.erase(it);
        } else {
            it++;
        }
    }
    garbages.insert(group);
    return groups.erase(it);
}

void Scene::RemoveGroup(Group *group) {
    RemoveGroup(groups.find(group));
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