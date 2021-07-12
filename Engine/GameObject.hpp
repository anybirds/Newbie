#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include <Transform.hpp>

class Component;
class Transform;
class Prefab;

CLASS_FINAL_ATTR(GameObject, Entity, ENGINE_EXPORT) {
    TYPE(GameObject);

    PROPERTY_NONE(std::vector<Component *>, components);
    PROPERTY_GET(Transform *, transform, Transform);
    PROPERTY(std::string, name, Name);

public:
    static void ToJson(nlohmann::json &js, const GameObject *gameObject);
    static void FromJson(const nlohmann::json &js, GameObject *&gameObject, bool nullify = true);

public:
    GameObject() : transform(nullptr) {}
    
    std::string &GetName() { return name; }
    const std::vector<Component *> &GetAllComponents() const { return components; }
    bool IsLocalEnabled() const { return transform->IsLocalEnabled(); }
    bool IsEnabled() const { return transform->IsEnabled(); }
    void SetLocalEnabled(bool localEnabled) { transform->SetLocalEnabled(localEnabled); }
    bool IsRemoved() const { return transform->IsRemoved(); }
    GameObject *GetParent() const { Transform *p = transform->GetParent(); if (p) { return p->GetGameObject(); } else { return nullptr; } }
    void SetParent(GameObject *parent) { transform->SetParent(parent->GetTransform()); }

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
    GameObject *AddGameObject(const nlohmann::json &js);
    GameObject *FindGameObject(const std::string &name) const;

    void Remove() { transform->flags |= Component::REMOVED; transform->localEnabled = false; }

    friend class Scene;
    friend class Component;
    friend class Transform;
};
