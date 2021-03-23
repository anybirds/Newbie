#include <Group.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>
#include <Scene.hpp>

using namespace std;
using namespace Engine;

Group::Group() {
    Scene &scene = Scene::GetInstance();
    scene.groups.push_back(this);
}

Group::~Group() {
    for (GameObject *gameObject : gameObjects) {
        delete gameObject;
    }
    Scene &scene = Scene::GetInstance();
    scene.groups.erase(find(scene.groups.begin(), scene.groups.end(), this));
}

GameObject *Group::GetGameObject(const string &name) const {
    for (GameObject *gameObject : gameObjects) {
        if (gameObject->IsRemoved()) {
            continue;
        }
        if (GameObject *found = gameObject->GetGameObject(name)) {
            return found;
        }
    }
    return nullptr;
}

GameObject *Group::AddGameObject(const string &name) {
    GameObject *gameObject = new GameObject();
    Transform *transform = gameObject->AddComponent<Transform>();

    gameObject->group = this;
    gameObject->name = name;
    gameObject->transform = transform;
    return gameObject;
}