#pragma once

#include <cstdint>

#include <Entity.hpp>

NAMESPACE(Engine) {

    class GameObject;
    class Transform;

    CLASS_ATTR(Component, Entity, ENGINE_EXPORT) {
        TYPE(Component);

        PROPERTY_NONE(bool, localEnabled);
        PROPERTY_GET(GameObject *, gameObject, GameObject);
        
    private:
        bool enabled;
        bool removed;

    public:
        Component() : enabled(false), removed(false), localEnabled(true), gameObject(nullptr) {}

        bool IsEnabled() const;
        bool IsLocalEnabled() const { return localEnabled; }
        void SetLocalEnabled(bool localEnabled) { this->localEnabled = localEnabled; }
        bool IsRemoved() const { return removed; } 
        
        Transform *GetTransform() const;
        
        virtual void OnTrack() {}
        virtual void OnUntrack() {}
        virtual void OnEnable() {}
        virtual void OnDisable() {}
        virtual void OnRemove();

        friend class Scene;
		friend class GameObject;
    };
}