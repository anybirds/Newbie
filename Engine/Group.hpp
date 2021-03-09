#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <Entity.hpp>
#include <Type.hpp>

namespace Engine {

    class GameObject;
    class IBehavior;
    class IEnable;
    class IRender;
    class IDraw;

    /*
    Unit of destruction. 
    */
    class ENGINE_EXPORT Group final : public Entity {
        TYPE(Group)

    private:
        std::unordered_map<std::string, GameObject *> gameObjects;
        std::vector<GameObject *> garbages;
        std::vector<IBehavior *> ibehaviors;
        std::vector<IEnable *> ienables;
        std::vector<IRender *> irenders;
        std::vector<IDraw *> idraws;

    public:
        ~Group();

        GameObject *GetGameObject(const std::string &name) const;
        GameObject *AddGameObject(const std::string &name);
        bool RemoveGameObject(const std::string &name) const;

        friend class GameObject;
        friend class Scene;
        friend class Camera;
    };
}