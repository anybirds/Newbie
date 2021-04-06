#pragma once

#include <cstdint>

#include <Entity.hpp>
#include <Type.hpp>

NAMESPACE(Engine) {

    class Group;
    class GameObject;
    class Transform;

    /*
    Components that organizes the GameObject.
    */
    CLASS_ATTR(Component, Entity, ENGINE_EXPORT) {
        TYPE(Component);

        PROPERTY_NONE(bool, localEnabled);
        PROPERTY_NONE(bool, enabled);
        PROPERTY_GET(GameObject *, gameObject, GameObject);
        
    public:
        Component() : localEnabled(true), gameObject(nullptr) {}

        bool IsLocalEnabled() { return localEnabled; }
        bool IsEnabled() { return enabled; }
        void SetLocalEnabled(bool localEnabled);
        
        Transform *GetTransform() const;
        Group *GetGroup() const;

		friend class GameObject;
        friend class Group;
    };
}