#include <Component.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>

using namespace std;

Transform *Component::GetTransform() const {
    return gameObject->GetTransform();
}

bool Component::IsEnabled() const {
    bool enabled = IsLocalEnabled();
    Transform *transform = GetTransform();
    while (transform) {
        enabled &= transform->IsLocalEnabled();
        transform = transform->GetParent();
    }
    return enabled;
}