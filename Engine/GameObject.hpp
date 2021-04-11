#pragma once

#include <string>
#include <unordered_set>

#include <Transform.hpp>
#include <Script.hpp>
#include <Graphics/Renderer.hpp>
#include <Graphics/Drawer.hpp>
#include <Scene.hpp>

NAMESPACE(Engine) {
    
    class Group;
    class Component;
    class Transform;

    /*
    Abstraction of an object in the scene.
    */
    CLASS_FINAL_ATTR(GameObject, Entity, ENGINE_EXPORT) {
        TYPE(GameObject);

        PROPERTY(std::string, name, Name);
        PROPERTY_GET(Group *, group, Group);
        PROPERTY_GET(Transform *, transform, Transform);
        PROPERTY_NONE(std::unordered_set<Component *>, components);

    private:
        std::unordered_set<Component *> garbages;

    public:
        GameObject() : group(nullptr), transform(nullptr) {}
        virtual ~GameObject();
        
        bool IsLocalEnabled() const { return transform->IsLocalEnabled(); }
        bool IsEnabled() const { return transform->IsEnabled(); }
        void SetLocalEnabled(bool localEnabled) { transform->SetLocalEnabled(localEnabled); }

        GameObject *GetParent() const { Transform *p = transform->GetParent(); if (p) { return p->GetGameObject(); } else { return nullptr; }}

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
        std::enable_if_t<std::is_base_of_v<Component, T> && !std::is_same_v<Transform, T> &&
        !std::is_base_of_v<Script, T> && !std::is_base_of_v<Drawer, T> && !std::is_base_of_v<Renderer, T>, bool> = true>
        T *AddComponent() {
            T *t = new T();
            t->gameObject = this;
            t->enabled = IsEnabled() && t->IsLocalEnabled();
            components.insert(t);
            return t;
        }
        template <class T, std::enable_if_t<std::is_same_v<Transform, T>, bool> = true>
        T *AddComponent() {
            if (transform) {
                return transform;
            }
            T *t = new T();
            t->gameObject = this;
            transform = t;
            components.insert(t);
            return t;
        } 
        template <class T, std::enable_if_t<std::is_base_of_v<Script, T>, bool> = true> 
        T *AddComponent() {
            T *t = new T();
            t->gameObject = this;
            t->enabled = IsEnabled();
            components.insert(t);
            Scene &scene = Scene::GetInstance();
            if (scene.IsLoaded()) {
               scene.scripts.insert((Script *)t);
            }
            return t;
        }
        template <class T, std::enable_if_t<std::is_base_of_v<Renderer, T>, bool> = true> 
        T *AddComponent() {
            T *t = new T();
            t->gameObject = this;
            t->enabled = IsEnabled();
            components.insert(t);
            Scene &scene = Scene::GetInstance();
            if (scene.IsLoaded()) {
               scene.renderers.insert((Renderer *)t);
            }
            return t;
        }
        template <class T, std::enable_if_t<std::is_base_of_v<Drawer, T>, bool> = true> 
        T *AddComponent() {
            T *t = new T();
            t->gameObject = this;
            t->enabled = IsEnabled();
            components.insert(t);
            Scene &scene = Scene::GetInstance();
            if (scene.IsLoaded()) {
               scene.drawers.insert((Drawer *)t);
            }
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
