#pragma once 

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <EngineExport.hpp>
#include <Component.hpp>
#include <Type.hpp>

namespace Engine {	

    /*
    Represents a single frame in 3D space that has position, rotation, scale and hierarchical parent information.
    */
    class ENGINE_EXPORT [[Serialize]] Transform final : [[Serialize]] public Component {
        TYPE(Transform)

    private:
        glm::vec3 localPosition;
        glm::quat localRotation;
        glm::vec3 localScale;
        glm::vec3 localEulerAngles;

        Transform *parent;
        std::vector<Transform *> children;

        mutable bool updated;
        mutable glm::mat4 localToWorldMatrix;

        void PropagateUpdate();

    public:
        Transform() : localRotation(1.0f, 0.0f, 0.0f, 0.0f), localToWorldMatrix(glm::mat4(1.0f)) {}

        virtual void SetRemoved() override;

        const glm::vec3 &GetLocalPosition() const { return localPosition; }
        const glm::quat &GetLocalRotation() const { return localRotation; }
        const glm::vec3 &GetLocalScale() const { return localScale; }
        const glm::vec3 &GetLocalEulerAngles() const { return localEulerAngles; }
        Transform *GetParent() const { return parent; }
        const std::vector<Transform *> &GetChildren() const { return children; }

        glm::mat4 GetLocalToWorldMatrix() const;
        glm::mat4 GetWorldToLocalMatrix() const;
        glm::vec3 GetPosition() const;
        glm::quat GetRotation() const;
        glm::vec3 GetScale() const;

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

        friend class Scene;
	};
}