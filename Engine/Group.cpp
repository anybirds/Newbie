#include <Group.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>

using namespace std;
using namespace Engine;

Group::~Group() {
    for (GameObject *gameObject : gameObjects) {
        delete gameObject;
    }
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

    gameObject->name = name;
    gameObject->transform = transform;
    return gameObject;
}