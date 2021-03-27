#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <Transform.hpp>
#include <GameObject.hpp>

using namespace std;
using namespace glm;
using namespace Engine;

void Transform::PropagateUpdate() {
    if (dirty) {
        return;
    }
    dirty = true;

    for (Transform *transform : children) {
        transform->PropagateUpdate();
    }
}

mat4 Transform::GetLocalToWorldMatrix() const {
    if (dirty) {
        dirty = false;

        mat4 T = glm::translate(mat4(1.0f), localPosition);
        quat R = localRotation;
        mat4 S = glm::scale(mat4(1.0f), localScale);
        mat4 P = parent? parent->GetLocalToWorldMatrix() : mat4(1.0f);
        localToWorldMatrix = P * T * toMat4(R) * S;
    }
    return localToWorldMatrix;
}

mat4 Transform::GetWorldToLocalMatrix() const {
    return inverse(GetLocalToWorldMatrix());
}

vec3 Transform::GetPosition() const {
    return mat3(parent? parent->GetLocalToWorldMatrix() : mat4(1.0f)) * localPosition;
}

quat Transform::GetRotation() const {
    return toQuat((parent? parent->GetLocalToWorldMatrix() : mat4(1.0f)) * toMat4(localRotation));
}

vec3 Transform::GetScale() const {
    return mat3(parent? parent->GetLocalToWorldMatrix() : mat4(1.0f)) * localScale;
}

void Transform::SetLocalPosition(const glm::vec3 &localPosition) {
    this->localPosition = localPosition;
    PropagateUpdate();
}

void Transform::SetLocalRotation(const glm::quat &localRotation) {
    this->localEulerAngles = degrees(eulerAngles(localRotation));
    this->localRotation = localRotation;
    PropagateUpdate();
}

void Transform::SetLocalScale(const glm::vec3 &localScale) {
    this->localScale = localScale;
    PropagateUpdate();
}

void Transform::SetLocalEulerAngles(const glm::vec3 &localEulerAngles) {
    this->localEulerAngles = localEulerAngles;
    this->localRotation = quat(radians(localEulerAngles));
    PropagateUpdate();
}

void Transform::SetPosition(const glm::vec3 &position) {
    this->localPosition = mat3(parent? parent->GetWorldToLocalMatrix() : mat4(1.0f)) * position;
    PropagateUpdate();
}

void Transform::SetScale(const glm::vec3 &scale) {
    this->localScale = mat3(parent? parent->GetWorldToLocalMatrix() : mat4(1.0f)) * scale;
    PropagateUpdate();
}

void Transform::SetParent(Transform *parent) {
    dirty = true;
    this->parent = parent;
}

void Transform::Rotate(const glm::vec3 &eulerAngles) {
    SetLocalEulerAngles(localEulerAngles + eulerAngles);
}

void Transform::RotateAround(const glm::vec3 &axis, float angle) {
    SetLocalRotation(rotate(GetLocalRotation(), radians(angle), axis));
}

