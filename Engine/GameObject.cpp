#include <cstdint>

#include <GameObject.hpp>
#include <Component.hpp>

using namespace std;
using namespace Engine;

GameObject *GameObject::AddGameObject() {
    GameObject *child = new GameObject();
    Transform *t = child->AddComponent<Transform>();
    t->parent = transform;
    return child;
}

GameObject *GameObject::AddGameObject(GameObject *gameObject) {

}

GameObject *GameObject::FindGameObject(const string &name) const {
    if (!IsRemoved() && GetName() == name) {
        return const_cast<GameObject *>(this);
    }
    for (Transform *t : transform->GetChildren()) {
        GameObject *child = t->GetGameObject();
        if (child->FindGameObject(name)) {
            return child;
        }
    }
    return nullptr;
}