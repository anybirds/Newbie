#include <functional>

#include <Component.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>
#include <Group.hpp>

using namespace std;
using namespace Engine;

Transform *Component::GetTransform() const {
    return gameObject->GetTransform();
}

Group *Component::GetGroup() const {
    return GetGameObject()->GetGroup();
}

void Component::SetLocalEnabled(bool localEnabled) {
    this->localEnabled = localEnabled;
    GetGroup()->dirty = true;
}