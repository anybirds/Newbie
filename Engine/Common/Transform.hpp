#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <Common/Component.hpp>

#include <engine_global.hpp>

namespace Engine {	

    SER_DECL(ENGINE_EXPORT, Transform)

    void ENGINE_EXPORT Destroy(Transform *transform);

    /*
    GameObject's position, rotation, scale and hierarchical parent information.
    */
    class ENGINE_EXPORT Transform final : public Component {
        TYPE_DECL(Transform)

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
        Transform(const std::string &name, Type *type = Transform::type);

        virtual void OnInit() override;

        GET(glm::vec3, LocalPosition, localPosition)
        GET(glm::quat, LocalRotation, localRotation)
        GET(glm::vec3, LocalScale, localScale)
        GET(glm::vec3, LocalEulerAngles, localEulerAngles)
        GET(Transform *, Parent, parent)

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

        void Rotate(const glm::vec3 &eulerAngles);
        void RotateAround(const glm::vec3 &axis, float angle);

        friend void Destroy(Transform *);
	};
}

typedef typename concat<TYPE_LIST, Engine::Transform>::type TypeListTransform;
#undef TYPE_LIST
#define TYPE_LIST TypeListTransform

#endif
