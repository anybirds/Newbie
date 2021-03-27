#pragma once

#include <cstdint>

#include <Entity.hpp>
#include <Type.hpp>

namespace Engine {

    class Group;
    class GameObject;
    class Transform;

    /*
    Components that organizes the GameObject.
    */
    class ENGINE_EXPORT [[Serialize]] Component : public Entity {
        TYPE(Component)

    private:
        bool localEnabled;
        bool enabled;
        GameObject *gameObject;

        void SetEnabled(bool enabled) { this->enabled = enabled; }

    public:
        Component() : gameObject(nullptr) {}
        
        bool IsLocalEnabled() { return localEnabled; }
        bool IsEnabled() { return enabled; }
        void SetLocalEnabled(bool localEnabled);
        
        GameObject *GetGameObject() const { return gameObject; }
        Transform *GetTransform() const;
        Group *GetGroup() const;

		friend class GameObject;
    };
}