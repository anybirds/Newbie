#include <Component.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>

using namespace Engine;

bool Component::IsEnabled() const {
    return enabled;
}

bool Component::SetEnabled() {
    localEnabled = true;
    Transform *transform = gameObject->GetTransform();
    for ()
    

    bool enabled = this->enabled;
    Transform *transform = gameObject->GetTransform();
    while (enabled && transform) {
        enabled = enabled && transform->enabled;
        transform = transform->GetParent();
    }
    enabled = true;
    return IsEnabled();
}