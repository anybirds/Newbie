#pragma once

#include <cstdint>

#include <Entity.hpp>

NAMESPACE(Engine) {

    class GameObject;
    class Transform;

    /*
    Component that organizes the GameObject.
    */
    CLASS_ATTR(Component, Entity, ENGINE_EXPORT) {
        TYPE(Component);

        PROPERTY_NONE(bool, localEnabled);
        PROPERTY_GET(GameObject *, gameObject, GameObject);
        
    private:
        bool enabled;
        uint8_t flags;
        enum : uint8_t {
            Add = 1, // to be added?
            Remove = 1 << 1, // to be removed?
        };
        void SetFlags(uint8_t flags) { this->flags |= flags; }
        void ClearFlags() { flags = 0U; }
        
    public:
        Component() : flags(Add), localEnabled(true), enabled(false), gameObject(nullptr) {}

        bool IsEnabled() const;
        bool IsLocalEnabled() const { return localEnabled; }
        void SetLocalEnabled(bool localEnabled) { this->localEnabled = localEnabled; }
        bool IsRemoved() const { return flags & Remove; } 
        
        Transform *GetTransform() const;
        
        virtual void OnEnable() {}
        virtual void OnDisable() {}
        virtual void OnAdd() {}
        virtual void OnRemove();

        friend class Scene;
		friend class GameObject;
    };
}