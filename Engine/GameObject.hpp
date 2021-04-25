#pragma once

#include <string>
#include <unordered_set>
#include <nlohmann/json.hpp>

#include <Transform.hpp>

NAMESPACE(Engine) {
    
    class Component;
    class Transform;

    CLASS_FINAL_ATTR(GameObject, Entity, ENGINE_EXPORT) {
        TYPE(GameObject);

        PROPERTY(std::string, name, Name);
        PROPERTY_GET(Transform *, transform, Transform);
        PROPERTY_NONE(std::unordered_set<Component *>, components);

    private:
        static void ToJson(nlohmann::json &js, std::unordered_set<GameObject *> &roots);
        static void FromJson(nlohmann::json &js, std::unordered_set<GameObject *> &roots);
        GameObject *GetCopy();

    public:
        GameObject() : transform(nullptr) {}
        
        std::string &GetName() { return name; }
        const std::unordered_set<Component *> &GetAllComponents() { return components; }
        bool IsLocalEnabled() const { return transform->IsLocalEnabled(); }
        bool IsEnabled() const { return transform->IsEnabled(); }
        void SetLocalEnabled(bool localEnabled) { transform->SetLocalEnabled(localEnabled); }
        bool IsRemoved() const { return transform->IsRemoved(); }
        GameObject *GetParent() const { Transform *p = transform->GetParent(); if (p) { return p->GetGameObject(); } else { return nullptr; } }

        template <class T, std::enable_if_t<std::is_base_of_v<Component, T>, bool> = true>
        T *GetComponent() const {
            for (Component *component : components) {
                T *t = dynamic_cast<T *>(component);
                if (!component->IsRemoved() && t) {
                    return t;
                }
            }
            return nullptr;
        }
        template <class T, std::enable_if_t<std::is_base_of_v<Component, T> && !std::is_same_v<Transform, T>, bool> = true> 
        T *AddComponent() {
            T *t = new T();
            t->gameObject = this;
            components.insert(t);
            return t;
        }
        template <class T, std::enable_if_t<std::is_same_v<Transform, T>, bool> = true> 
        T *AddComponent() {
            // prohibit adding multiple transforms
            if (transform) {
                return transform;
            }
            T *t = new T();
            t->gameObject = this;
            components.insert(t);
            transform = t;
            return t;
        }

        GameObject *AddGameObject();
        GameObject *AddGameObject(GameObject *gameObject);
        GameObject *FindGameObject(const std::string &name) const;

        friend class Scene;
        friend class Component;
        friend class Transform;
    };
}
