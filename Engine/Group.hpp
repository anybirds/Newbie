#pragma once

#include <string>
#include <set>
#include <unordered_set>

#include <Entity.hpp>
#include <Type.hpp>
#include <Script.hpp>
#include <Graphics/Renderer.hpp>
#include <Graphics/Drawer.hpp>

namespace Engine {

    class GameObject;
    class Component;

    /*
    Unit of destruction. 
    */
    class ENGINE_EXPORT [[Serialize]] Group final : public Entity {
        TYPE(Group)

    private:
        bool dirty;

        std::unordered_set<GameObject *> gameObjects;
        std::unordered_set<Script *> scripts;
        std::multiset<Renderer *, RendererComparer> renderers;
        std::multiset<Drawer *, DrawerComparer> drawers;

        [[NoSerialize]]
        std::unordered_set<GameObject *> garbages;

        void Refresh();

    public:
        Group();
        virtual ~Group();

        GameObject *FindGameObject(const std::string &name) const;
        GameObject *AddGameObject();
        void RemoveGameObject(GameObject *gameObject);

        friend class Scene;
        friend class Component;
        friend class GameObject;
        friend class Drawer;
        friend class Renderer;
        friend class Camera;
    };
}