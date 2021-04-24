#include <cstdint>

#include <GameObject.hpp>
#include <Component.hpp>

using namespace std;
using namespace Engine;

Component *GameObject::AddComponent(Component *component) {
    if (dynamic_cast<Transform *>(component)) {
        return transform;
    }

    Component *t = (Component *)component->GetCopy();
    t->gameObject = this;
    t->enabled = false;
    t->OnAdd();
    components.insert(t);
    return t;
}

GameObject *GameObject::AddGameObject() {
    GameObject *child = new GameObject();
    Transform *t = child->AddComponent<Transform>();
    t->parent = transform;
    transform->children.insert(t);
    return child;
}

GameObject *GameObject::AddGameObject(GameObject *gameObject) {
    GameObject *add = new GameObject();
    add->SetName(gameObject->GetName());
    add->AddComponent<Transform>();
    for (Component *component : gameObject->components) {
        add->AddComponent(component);
    }
    for (Transform *t : gameObject->GetTransform()->GetChildren()) {
        add->AddGameObject(t->GetGameObject());
    }
    add->transform->parent = transform;
    transform->children.insert(add->transform);
    return add;
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