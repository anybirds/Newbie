#include <GameObject.hpp>
#include <Component.hpp>

using namespace std;
using namespace Engine;

GameObject::~GameObject() {
    for (Transform *t : transform->GetChildren()) {
        delete t->GetGameObject();
    }
    for (Transform *t : transform->garbages) {
        delete t->GetGameObject();
    }
    for (Component *component : components) {
        delete component;
    }
}

void GameObject::RemoveComponent(Component *component) {
    if (this == component->GetGameObject() && Transform::StaticType() != component->GetType()) {
        components.erase(component);
        garbages.insert(component);
        if (Script *script = dynamic_cast<Script *>(component)) {
            group->scripts.erase(script);
        } else if (Renderer *renderer = dynamic_cast<Renderer *>(component)) {
            group->renderers.erase(renderer);
        } else if (Drawer *drawer = dynamic_cast<Drawer *>(component)) {
            group->drawers.erase(drawer);
        }
    }
}

GameObject *GameObject::FindGameObject(const string &name) const {
    if (GetName() == name) {
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

GameObject *GameObject::AddGameObject() {
    GameObject *child = new GameObject();
    Transform *t = child->AddComponent<Transform>();
    t->SetParent(transform);

    child->group = group;
    child->transform = t;
    return child;
}

void GameObject::RemoveGameObject(GameObject *gameObject) {
    transform->children.erase(gameObject->transform);
    transform->garbages.insert(gameObject->transform);
}