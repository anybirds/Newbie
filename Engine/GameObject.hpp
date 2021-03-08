#pragma once

#include <string>
#include <vector>

#include <Entity.hpp>
#include <Type.hpp>
#include <Script.hpp>
#include <Graphics/Renderer.hpp>

namespace Engine {
    
    class Group;
    class Component;
    class Transform;

    /*
    Abstraction of an object in the scene.
    */
    class ENGINE_EXPORT GameObject final : public Entity {
        TYPE(GameObject);

    private:
        bool enabled;
        std::string name;
        Group *group;
        Transform *transform;
        std::vector<Component *> components;
    
    public:
        ~GameObject();

        bool IsEnabled() const { return enabled; }
        const std::string &GetName() const { return name; }
        Group *GetGroup() const { return group; }

        void SetEnabled() { enabled = true; }
        void SetName(const std::string &name) { this->name = name; }

        template <class T, std::enable_if_t<std::is_base_of_v<Component, T>, bool> = true>
        T *GetComponent() const {
            for (Component *component : components) {
                T *t = dynamic_cast<T *>(component);
                if (t && !(t->IsRemoved())) {
                    return t;
                }
            }
            return nullptr;
        }
        template <class T, std::enable_if_t<std::is_base_of_v<Component, T>, bool> = true>
        T *AddComponent() const {
            T *component = new T();
            component->gameObject = this;
            components.push_back(component);
            if (std::is_base_of_v<Renderer, T>) {
                group->renderers.push_back(component);   
            } else if (std::is_base_of_v<Script, T>) {
                group->scripts.push_back(component);
            }
            return component;
        }
        bool RemoveComponent(Component *component) const {
            component->SetRemoved();
            return true;
        }
        template <class T, std::enable_if_t<std::is_base_of_v<Component, T>, bool> = true>
        bool RemoveComponent() const {
            for (Component *component : components) {
                T *t = dynamic_cast<T *>(component);
                if (t && !(t->IsRemoved())) {
                    t->SetRemoved();
                    return true;
                }
            }
            return false;
        }

        friend class Group;
        friend class Scene;
    };
}
