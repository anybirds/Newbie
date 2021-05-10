#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <Transform.hpp>
#include <GameObject.hpp>
#include <Scene.hpp>

using namespace std;
using namespace glm;

Transform::Transform() :
    localPosition(0.0f), localRotation(1.0f, 0.0f, 0.0f, 0.0f), localScale(1.0f),
    localEulerAngles(0.0f), parent(nullptr), dirty(true), 
    localToWorldMatrix(glm::mat4(1.0f)) {}
    
Transform::~Transform() {
    delete GetGameObject(); // GameObject follows Transform
}

void Transform::Propagate() {
    if (dirty) {
        return;
    }
    dirty = true;

    for (Transform *transform : children) {
        transform->Propagate();
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

vec3 Transform::GetRight() const {
    return normalize(vec3(GetLocalToWorldMatrix()[0]));
}

vec3 Transform::GetUp() const {
    return normalize(vec3(GetLocalToWorldMatrix()[1]));
}

vec3 Transform::GetForward() const {
    return normalize(vec3(GetLocalToWorldMatrix()[2]));
}

void Transform::SetLocalPosition(const glm::vec3 &localPosition) {
    this->localPosition = localPosition;
    Propagate();
}

void Transform::SetLocalRotation(const glm::quat &localRotation) {
    this->localEulerAngles = degrees(eulerAngles(localRotation));
    this->localRotation = localRotation;
    Propagate();
}

void Transform::SetLocalScale(const glm::vec3 &localScale) {
    this->localScale = localScale;
    Propagate();
}

void Transform::SetLocalEulerAngles(const glm::vec3 &localEulerAngles) {
    this->localEulerAngles = localEulerAngles;
    this->localRotation = quat(radians(localEulerAngles));
    Propagate();
}

void Transform::SetPosition(const glm::vec3 &position) {
    this->localPosition = mat3(parent? parent->GetWorldToLocalMatrix() : mat4(1.0f)) * position;
    Propagate();
}

void Transform::SetScale(const glm::vec3 &scale) {
    this->localScale = mat3(parent? parent->GetWorldToLocalMatrix() : mat4(1.0f)) * scale;
    Propagate();
}

void Transform::SetParent(Transform *parent) {
    // prevent cycles in hierarchy
    Transform *p = parent;
    while (p) {
        if (this == p) {
            return;
        }
        p = p->parent;
    }
    
    Scene &scene = Scene::GetInstance();
    if (this->parent) {
        this->parent->children.erase(find(this->parent->children.begin(), this->parent->children.end(), this));
    } else {
        scene.roots.erase(find(scene.roots.begin(), scene.roots.end(), GetGameObject()));
    }
    if (parent) {
        parent->children.push_back(this);
    } else {
        scene.roots.push_back(GetGameObject());
    }
    this->parent = parent;
    Propagate();
}

void Transform::Rotate(const glm::vec3 &eulerAngles) {
    SetLocalEulerAngles(localEulerAngles + eulerAngles);
}

void Transform::RotateAround(const glm::vec3 &axis, float angle) {
    SetLocalRotation(rotate(GetLocalRotation(), radians(angle), axis));
}

void Transform::Translate(const glm::vec3 &translation) {
    SetLocalPosition(localPosition + vec3(GetLocalToWorldMatrix() * vec4(translation, 0.0f)));
}

GameObject *Transform::AddGameObject() {
    GameObject *child = new GameObject();
    Transform *t = child->AddComponent<Transform>();
    t->parent = this;
    children.push_back(t);
    return child;
}

GameObject *Transform::AddGameObject(GameObject *gameObject) {

}

GameObject *Transform::AddGameObject(const std::shared_ptr<Prefab> &prefab) {

}

GameObject *Transform::FindGameObject(const string &name) const {
    for (Transform *t : GetChildren()) {
        GameObject *child = t->GetGameObject();
        if (child->GetName() == name) {
            return child;
        }
        t->FindGameObject(name);
    }
    return nullptr;
}
