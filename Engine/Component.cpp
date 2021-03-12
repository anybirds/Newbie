#include <Component.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>

using namespace Engine;

bool Component::IsEnabled() const {
    bool enabled = this->enabled;
    Transform *transform = gameObject->GetTransform();
    while (enabled && transform) {
        enabled = enabled && transform->enabled;
        transform = transform->GetParent();
    }
    return enabled;
}

bool Component::SetEnabled() {
    enabled = true;
    return IsEnabled();
}