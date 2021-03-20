#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <Entity.hpp>
#include <Type.hpp>

namespace Engine {

    class GameObject;
    class Component;

    /*
    Unit of destruction. 
    */
    class ENGINE_EXPORT [[Serialize]] Group final : public Entity {
        TYPE(Group)

    private:
        std::vector<GameObject *> gameObjects;
        std::vector<Component *> ibehaviors;
        std::vector<Component *> irenders;
        std::vector<Component *> idraws;

    public:
        ~Group();

        GameObject *GetGameObject(const std::string &name) const;
        GameObject *AddGameObject(const std::string &name);

        friend class GameObject;
        friend class Scene;
        friend class Camera;
    };
}