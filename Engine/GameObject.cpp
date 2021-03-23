#include <GameObject.hpp>
#include <Component.hpp>

using namespace std;
using namespace Engine;

GameObject::~GameObject() {
    for (Transform *t : transform->GetChildren()) {
        delete t->GetGameObject();
    }
    for (Component *component : components) {
        delete component;
    }
}

GameObject *GameObject::GetGameObject(const string &name) {
    if (GetName() == name) {
        return this;
    }
    for (Transform *t : transform->GetChildren()) {
        GameObject *child = t->GetGameObject();
        if (!t->IsRemoved() && !child->IsRemoved() && child->GetGameObject(name)) {
            return child;
        }
    }
    return nullptr;
}

GameObject *GameObject::AddGameObject(const string &name) {
    GameObject *child = new GameObject();
    Transform *t = child->AddComponent<Transform>();
    t->SetParent(transform);

    child->group = group;
    child->name = name;
    child->transform = t;
    return child;
}

void GameObject::SetRemoved() {
    if (IsRemoved()) {
        return;
    }
    Entity::SetRemoved();
    for (Component *component : components) {
        component->SetRemoved();
    }
}