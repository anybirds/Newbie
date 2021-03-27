#pragma once

#include <string>
#include <unordered_set>

#include <Transform.hpp>
#include <Script.hpp>
#include <Graphics/Renderer.hpp>
#include <Graphics/Drawer.hpp>
#include <Group.hpp>

namespace Engine {
    
    class Group;
    class Component;
    class Transform;

    /*
    Abstraction of an object in the scene.
    */
    class ENGINE_EXPORT [[Serialize]] GameObject final : public Entity {
        TYPE(GameObject);

    private:
        std::string name;
        Group *group;
        Transform *transform;
        std::unordered_set<Component *> components;

        [[NoSerialize]]
        std::unordered_set<Component *> garbages;

    public:
        virtual ~GameObject();
        
        bool IsLocalEnabled() const { return transform->IsLocalEnabled(); }
        bool IsEnabled() const { return transform->IsEnabled(); }
        const std::string &GetName() const { return name; }
        Group *GetGroup() const { return group; }
        Transform *GetTransform() const { return transform; }
        GameObject *GetParent() const { Transform *p = transform->GetParent(); if (p) { return p->GetGameObject(); } else { return nullptr; }}

        void SetLocalEnabled(bool localEnabled) { transform->SetLocalEnabled(localEnabled); }
        void SetName(const std::string &name) { this->name = name; }

        template <class T, std::enable_if_t<std::is_base_of_v<Component, T>, bool> = true>
        T *GetComponent() const {
            for (Component *component : components) {
                if (T *t = dynamic_cast<T *>(component)) {
                    return t;
                }
            }
            return nullptr;
        }
        template <class T, 
        std::enable_if_t<std::is_base_of_v<Component, T> && 
        !std::is_base_of_v<Script, T> && !std::is_base_of_v<Drawer, T> && !std::is_base_of_v<Renderer, T>, bool> = true>
        T *AddComponent() {
            T *t = new T();
            t->gameObject = this;
            components.insert(t);
            return t;
        }
        template <class T, std::enable_if_t<std::is_base_of_v<Script, T>, bool> = true> 
        T *AddComponent() {
            T *t = new T();
            t->gameObject = this;
            components.insert(t);
            group->scripts.insert((Script *)t);
            return t;
        }
        template <class T, std::enable_if_t<std::is_base_of_v<Renderer, T>, bool> = true> 
        T *AddComponent() {
            T *t = new T();
            t->gameObject = this;
            components.insert(t);
            group->renderers.insert((Renderer *)t);
            return t;
        }
        template <class T, std::enable_if_t<std::is_base_of_v<Drawer, T>, bool> = true> 
        T *AddComponent() {
            T *t = new T();
            t->gameObject = this;
            components.insert(t);
            group->drawers.insert((Drawer *)t);
            return t;
        }
        void RemoveComponent(Component *component);

        GameObject *FindGameObject(const std::string &name) const;
        GameObject *AddGameObject();
        void RemoveGameObject(GameObject *gameObject);

        friend class Group;
        friend class Scene;
        friend class Component;
    };
}
