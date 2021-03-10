#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <Entity.hpp>
#include <Type.hpp>

namespace Engine {

    class GameObject;
    class IBehavior;
    class IRender;
    class IDraw;

    /*
    Unit of destruction. 
    */
    class ENGINE_EXPORT Group final : public Entity {
        TYPE(Group)

    private:
        std::vector<GameObject *> gameObjects;
        std::vector<IBehavior *> ibehaviors;
        std::vector<IRender *> irenders;
        std::vector<IDraw *> idraws;

    public:
        ~Group();

        GameObject *GetGameObject(const std::string &name) const;
        GameObject *AddGameObject(const std::string &name);

        friend class GameObject;
        friend class Scene;
        friend class Camera;
    };
}