#pragma once

#include <string>
#include <vector>

#include <Entity.hpp>
#include <Type.hpp>
#include <Transform.hpp>
#include <IBehavior.hpp>
#include <IRender.hpp>
#include <IDraw.hpp>
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
        std::vector<Component *> components;

    public:
        virtual ~GameObject();

        virtual void SetRemoved() override;

        bool IsEnabled() const { return transform->IsEnabled(); }
        const std::string &GetName() const { return name; }
        Group *GetGroup() const { return group; }
        Transform *GetTransform() const { return transform; }

        bool SetEnabled() { return transform->SetEnabled(); }
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
        T *AddComponent() {
            T *component = new T();
            component->gameObject = this;
            components.push_back(component); 
            if (std::is_base_of_v<IBehavior, T>) {
                group->ibehaviors.push_back(dynamic_cast<IBehavior *>(component));
            }
            if (std::is_base_of_v<IRender, T>) {
                group->irenders.push_back(dynamic_cast<IRender *>(component));
            }
            if (std::is_base_of_v<IDraw, T>) {
                group->idraws.push_back(dynamic_cast<IDraw *>(component));
            }
            return component;
        }

        GameObject *GetGameObject(const std::string &name);
        GameObject *AddGameObject(const std::string &name);

        friend class Group;
        friend class Scene;
    };
}
