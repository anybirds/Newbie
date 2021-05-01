#pragma once

#include <cstdint>

#include <Entity.hpp>

NAMESPACE(Engine) {

    class GameObject;
    class Transform;

    CLASS_ATTR(Component, Entity, ENGINE_EXPORT) {
        TYPE(Component);

        PROPERTY_NONE(uint8_t, state);
        PROPERTY_GET(GameObject *, gameObject, GameObject);
        
    private:
        enum : uint8_t {
            LOCAL_ENABLED = 1,
            REMOVED = 1 << 2,
            ENABLED = 1,
            DESTROYED = 1 << 2
        };
        uint8_t flags;

    public:
        Component() : flags(0U), state(0U), gameObject(nullptr) {}

        bool IsEnabled() const;
        bool IsLocalEnabled() const { return state & LOCAL_ENABLED; }
        void SetLocalEnabled(bool localEnabled) { state = (state & ~LOCAL_ENABLED) + localEnabled; }
        bool IsRemoved() const { return state & REMOVED; } 
        
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