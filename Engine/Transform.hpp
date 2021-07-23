#pragma once 

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <Component.hpp>

class Prefab;

CLASS_FINAL_ATTR(Transform, Component, ENGINE_EXPORT) {
    TYPE(Transform);

    PROPERTY_NONE(std::vector<Transform *>, children);
    PROPERTY_NONE(Transform *, parent);
    PROPERTY(glm::vec3, localPosition, LocalPosition);
    PROPERTY(glm::vec3, localEulerAngles, LocalEulerAngles);
    PROPERTY(glm::vec3, localScale, LocalScale);
    PROPERTY_NONE(glm::quat, localRotation);

private:
    mutable bool dirty;
    mutable glm::mat4 localToWorldMatrix;
    void Propagate();

public:
    Transform();
    virtual ~Transform();

    const std::vector<Transform *> &GetChildren() const { return children; }
    Transform *GetParent() const { return parent; }
    const glm::quat &GetLocalRotation() const { return localRotation; }
    glm::mat4 GetLocalToWorldMatrix() const;
    glm::mat4 GetWorldToLocalMatrix() const;
    glm::vec3 GetPosition() const;
    glm::quat GetRotation() const;
    glm::vec3 GetScale() const;
    glm::vec3 GetRight() const;
    glm::vec3 GetUp() const;
    glm::vec3 GetForward() const;

    void SetLocalPosition(const glm::vec3 &localPosition);
    void SetLocalRotation(const glm::quat &localRotation);
    void SetLocalScale(const glm::vec3 &localScale);
    void SetLocalEulerAngles(const glm::vec3 &localEulerAngles);
    void SetPosition(const glm::vec3 &position);
    void SetRotation(const glm::quat &rotation);
    void SetScale(const glm::vec3 &scale);
    void SetEulerAngles(const glm::vec3 &eulerAngles);
    bool SetParent(Transform *parent);
    bool SetSibling(Transform *sibling);

    void Rotate(const glm::vec3 &eulerAngles);
    void RotateAround(const glm::vec3 &axis, float angle);
    void Translate(const glm::vec3 &translation);

    friend class Scene;
    friend class GameObject;
};