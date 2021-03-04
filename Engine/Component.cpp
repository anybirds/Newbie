#include <Common/Component.hpp>
#include <Common/GameObject.hpp>
#include <Common/Scene.hpp>

using namespace Engine;

namespace Engine {
    TYPE_DEF(Component)
    SER_DEF(Component, Object,
    MEMBER_SER, GameObject *, gameObject
    )
}


Component::Component(const std::string &name, Type *type) : Object(name, type) {
    Scene::curr->compset.insert(this);
}

Component::~Component() {
    Scene::curr->compset.erase(this);

    if (IsValid(gameObject)) {
        gameObject->components.erase(GetType(this)->GetName());
    }
}
