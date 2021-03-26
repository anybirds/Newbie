#pragma once

#include <string>
#include <set>
#include <unordered_map>
#include <vector>

#include <Entity.hpp>
#include <Type.hpp>
#include <IRender.hpp>

namespace Engine {

    class GameObject;
    class Component;
    class IBehavior;
    class IRender;
    class IDraw;
    class IRenderCompare;

    /*
    Unit of destruction. 
    */
    class ENGINE_EXPORT [[Serialize]] Group final : public Entity {
        TYPE(Group)

    private:
        std::vector<GameObject *> gameObjects;
        std::vector<IBehavior *> ibehaviors;
        std::multiset<IRender *, IRenderCompare> irenders;
        std::vector<IDraw *> idraws;

    public:
        Group();
        virtual ~Group();

        GameObject *GetGameObject(const std::string &name) const;
        GameObject *AddGameObject(const std::string &name);

        friend class GameObject;
        friend class Scene;
        friend class Camera;
    };
}