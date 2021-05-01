#pragma once

#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

#include <Transform.hpp>

NAMESPACE(Engine) {
    
    class Component;
    class Transform;
    class Prefab;

    CLASS_FINAL_ATTR(GameObject, Entity, ENGINE_EXPORT) {
        TYPE(GameObject);

        PROPERTY_NONE(std::vector<Component *>, components);
        PROPERTY(std::string, name, Name);
        PROPERTY_GET(Transform *, transform, Transform);
        PROPERTY(std::shared_ptr<Prefab>, prefab, Prefab);

    public:
        static void ToJson(nlohmann::json &js, const std::vector<GameObject *> &roots, bool nullify = false, const std::shared_ptr<Prefab> &base = nullptr, uintptr_t start = 0U);
        static uintptr_t FromJson(const nlohmann::json &js, std::vector<GameObject *> &roots, bool nullify = true);
        GameObject *GetCopy();

    public:
        GameObject() : transform(nullptr) {}
        
        std::string &GetName() { return name; }
        const std::vector<Component *> &GetAllComponents() { return components; }
        bool IsLocalEnabled() const { return transform->IsLocalEnabled(); }
        bool IsEnabled() const { return transform->IsEnabled(); }
        void SetLocalEnabled(bool localEnabled) { transform->SetLocalEnabled(localEnabled); }
        bool IsRemoved() const { return transform->IsRemoved(); }
        GameObject *GetParent() const { Transform *p = transform->GetParent(); if (p) { return p->GetGameObject(); } else { return nullptr; } }
        void SetParent(GameObject *parent) { transform->SetParent(parent->GetTransform()); }

        bool HasPrefabCycle() const;

        template <class T, std::enable_if_t<std::is_base_of_v<Component, T>, bool> = true>
        T *GetComponent() const {
            for (Component *component : components) {
                T *t = dynamic_cast<T *>(component);
                if (t) { return t; }
            }
            return nullptr;
        }
        template <class T, std::enable_if_t<std::is_base_of_v<Component, T> && !std::is_same_v<Transform, T>, bool> = true> 
        T *AddComponent() {
            T *t = new T();
            t->gameObject = this;
            components.push_back(t);
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
            components.push_back(t);
            transform = t;
            return t;
        }

        GameObject *AddGameObject();
        GameObject *AddGameObject(GameObject *gameObject);
        GameObject *AddGameObject(const std::shared_ptr<Prefab> &prefab);
        GameObject *FindGameObject(const std::string &name) const;

        friend class Scene;
        friend class Component;
        friend class Transform;
        friend class APrefab;
    };
}
