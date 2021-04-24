#pragma once 

#include <unordered_set>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <EngineExport.hpp>
#include <Component.hpp>

NAMESPACE(Engine) {	
    CLASS_FINAL_ATTR(Transform, Component, ENGINE_EXPORT) {
        TYPE(Transform);

        PROPERTY_GET(glm::vec3, localPosition, LocalPosition);
        PROPERTY_GET(glm::quat, localRotation, LocalRotation);
        PROPERTY_GET(glm::vec3, localScale, LocalScale);
        PROPERTY_GET(glm::vec3, localEulerAngles, LocalEulerAngles);
        PROPERTY_GET(Transform *, parent, Parent);
        PROPERTY_GET(std::unordered_set<Transform *>, children, Children);
        PROPERTY_NONE_ATTR(bool, dirty, mutable);
        PROPERTY_NONE_ATTR(glm::mat4, localToWorldMatrix, mutable);

    private:
        void Propagate();

    public:
        Transform();
        virtual ~Transform();

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
        // void SetRotation(const glm::quat &rotation);
        void SetScale(const glm::vec3 &scale);
        void SetParent(Transform *parent);

        void Rotate(const glm::vec3 &eulerAngles);
        void RotateAround(const glm::vec3 &axis, float angle);
        void Translate(const glm::vec3 &translation);
        
        friend class Scene;
        friend class GameObject;
	};
}