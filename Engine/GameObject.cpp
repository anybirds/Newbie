#include <cstdint>

#include <GameObject.hpp>
#include <Component.hpp>

using namespace std;
using json = nlohmann::json;

GameObject *GameObject::Instantiate(const json &js) { 
    
}

GameObject *GameObject::AddGameObject() {
    GameObject *child = new GameObject();
    Transform *t = child->AddComponent<Transform>();
    t->parent = transform;
    transform->children.push_back(t);
    return child;
}

GameObject *GameObject::AddGameObject(GameObject *gameObject) {

}

GameObject *GameObject::AddGameObject(const std::shared_ptr<Prefab> &prefab) {

}

GameObject *GameObject::FindGameObject(const string &name) const {
    if (GetName() == name) {
        return const_cast<GameObject *>(this);
    }
    for (Transform *t : transform->GetChildren()) {
        GameObject *child = t->GetGameObject();
        child->FindGameObject(name);
    }
    return nullptr;
}

void GameObject::Destroy() {
    Transform *parent = transform->GetParent();
    if (parent) {
        parent->children.erase(find(parent->children.begin(), parent->children.end(), this));
    }

    for (Transform *t : transform->GetChildren()) {
        t->GetGameObject()->Destroy();
    }

    auto temp = components;
    for (Component *component : temp) {
        delete component;
    }
}