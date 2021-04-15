#pragma once

#include <string>
#include <unordered_set>

#include <Entity.hpp>
#include <Type.hpp>
#include <Script.hpp>
#include <Graphics/Renderer.hpp>
#include <Graphics/Drawer.hpp>

NAMESPACE(Engine) {

    class GameObject;
    class Component;

    /*
    Unit of destruction. 
    */
    CLASS_FINAL_ATTR(Group, Entity, ENGINE_EXPORT) {
        TYPE(Group);

        PROPERTY_NONE(bool, dirty);
        PROPERTY(std::string, name, Name);
        PROPERTY_NONE(std::unordered_set<GameObject *>, gameObjects);

    private:
        std::unordered_set<GameObject *> garbages;
        
        void Refresh();

    public:
        virtual ~Group();

        GameObject *FindGameObject(const std::string &name) const;
        GameObject *AddGameObject();
        void RemoveGameObject(GameObject *gameObject);

        friend class Scene;
        friend class Component;
        friend class GameObject;
        friend class Transform;
    };
}