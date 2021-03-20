#pragma once

#include <Entity.hpp>
#include <Type.hpp>

namespace Engine {

    class GameObject;

    /*
    Components that organizes the GameObject.
    */
    class ENGINE_EXPORT [[Serialize]] Component : public Entity {
        TYPE(Component)

    private:
        bool enabled;
        GameObject *gameObject;
    
    public:
        bool IsEnabled() const;
        GameObject *GetGameObject() const { return gameObject; }

        bool SetEnabled();

		friend class GameObject;
    };
}