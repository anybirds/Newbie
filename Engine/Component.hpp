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
        enum : uint8_t {
            ENABLED = 1,
            REMOVED = 1 << 1,
            MARKED = 1 << 2
        };
        uint8_t flags;

    public:
        Component() : flags(0U), localEnabled(true), gameObject(nullptr) {}

        bool IsEnabled() const;
        bool IsLocalEnabled() const { return localEnabled; }
        void SetLocalEnabled(bool localEnabled) { this->localEnabled = localEnabled; }
        bool IsRemoved() const { return flags & REMOVED; } 
        
        Transform *GetTransform() const;
        
        virtual void OnTrack() {}
        virtual void OnUntrack() {}
        virtual void OnEnable() {}
        virtual void OnDisable() {}
        virtual void OnDestroy() {}

        friend class Scene;
		friend class GameObject;
    };
}