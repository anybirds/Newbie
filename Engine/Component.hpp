#pragma once

#include <cstdint>

#include <Entity.hpp>

class GameObject;
class Transform;

CLASS_ATTR(Component, Entity, ENGINE_EXPORT) {
    TYPE(Component);

    PROPERTY_BOOL(bool, localEnabled, LocalEnabled);
    PROPERTY_NONE(GameObject *, gameObject);
    
private:
    enum : uint8_t {
        ENABLED = 1,
        REMOVED = 1 << 1,
        DESTROYED = 1 << 2,
    };
    uint8_t flags;

public:
    Component() : localEnabled(true), flags(0U), gameObject(nullptr) {}

    bool IsEnabled() const;
    bool IsRemoved() const { return flags & REMOVED; } 
    void Remove() { flags |= REMOVED; localEnabled = false; }
    
    GameObject *GetGameObject() const { return gameObject; }
    Transform *GetTransform() const;
    
    virtual void OnTrack() {}
    virtual void OnUntrack() {}
    virtual void OnEnable() {}
    virtual void OnDisable() {}
    virtual void OnDestroy() {}

    friend class Scene;
    friend class GameObject;
};