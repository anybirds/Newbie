#include <Group.hpp>
#include <GameObject.hpp>

using namespace std;
using namespace Engine;

Group::~Group() {
    for (GameObject *gameObject : garbages) {
        delete gameObject;
    }
    for (auto &p : gameObjects) {
        delete p.second;
    }
}

GameObject *Group::GetGameObject(const string &name) const {
    auto it = gameObjects.find(name);
    if (it != gameObjects.end() && !(it->second->IsRemoved())) {
        return it->second;
    } else {
        return nullptr;
    }
}

GameObject *Group::AddGameObject(const string &name) {
    auto it = gameObjects.find(name);
    if (it != gameObjects.end()) {
        if (it->second->IsRemoved()) {
            garbages.push_back(it->second);
            gameObjects.erase(it);
        } else {
            return nullptr;
        }
    }
    GameObject *gameObject = new GameObject();
    gameObject->name = name;
    gameObject->group = this;
    gameObjects.insert({name, gameObject});
    return gameObject;
}

bool Group::RemoveGameObject(const string &name) {
    auto it = gameObjects.find(name);
    if (it != gameObjects.end()) {
        it->second->SetRemoved();
        return true;
    } else {
        return false;
    }
}
