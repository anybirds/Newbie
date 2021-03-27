#include <functional>

#include <Group.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>
#include <Scene.hpp>

using namespace std;
using namespace Engine;

void Group::Refresh() {
    std::function<void(GameObject *, bool)> recurse = [&recurse](GameObject *gameObject, bool enabled) {
        enabled = enabled && gameObject->IsLocalEnabled();
        for (Component *component : gameObject->components) {
            component->SetLocalEnabled(enabled && component->IsLocalEnabled());
        }
        Transform *transform = gameObject->GetTransform();
        for (Transform *t : transform->GetChildren()) {
            recurse(t->GetGameObject(), enabled);
        }
    }; 
    for (GameObject *gameObject : gameObjects) {  
        recurse(gameObject, true);
    }

    dirty = false;
}

Group::Group() {
    Scene &scene = Scene::GetInstance();
    scene.groups.insert(this);
}

Group::~Group() {
    for (GameObject *gameObject : gameObjects) {
        delete gameObject;
    }
    for (GameObject *garbage : garbages) {
        delete garbage;
    }
    Scene &scene = Scene::GetInstance();
    scene.groups.erase(this);
}

GameObject *Group::FindGameObject(const string &name) const {
    for (GameObject *gameObject : gameObjects) {
        if (GameObject *found = gameObject->FindGameObject(name)) {
            return found;
        }
    }
    return nullptr;
}

GameObject *Group::AddGameObject() {
    GameObject *gameObject = new GameObject();
    Transform *transform = gameObject->AddComponent<Transform>();
    
    gameObject->group = this;
    gameObject->transform = transform;

    gameObjects.insert(gameObject);
    
    return gameObject;
}

void Group::RemoveGameObject(GameObject *gameObject) {
    if (this == gameObject->group) {
        GameObject *parent = gameObject->GetParent();
        if (parent) {
            parent->RemoveGameObject(gameObject);
        } else {
            gameObjects.erase(gameObject);
        }
        garbages.insert(gameObject);
    }
}