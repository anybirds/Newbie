#pragma once

#include <Entity.hpp>

namespace Engine {

    class GameObject;

    /*
    Components that organizes the GameObject.
    */
    class ENGINE_EXPORT Component : public Entity {
    private:
        bool enabled;
        GameObject *gameObject;
    
    public:
        bool IsEnabled() const { return enabled; }
        GameObject *GetGameObject() const { return gameObject; }

        void SetEnabled() { enabled = true; }
        void SetGameObject(GameObject *gameObject) { this->gameObject = gameObject; }
    };
}