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

    const std::vector<Component *> &GetAllComponents() const { return components; }
    bool IsLocalEnabled() const { return transform->IsLocalEnabled(); }
    bool IsEnabled() const { return transform->IsEnabled(); }
    void SetLocalEnabled(bool localEnabled) { transform->SetLocalEnabled(localEnabled); }
    bool IsRemoved() const { return transform->IsRemoved(); }
    GameObject *GetParent() const { Transform *p = transform->GetParent(); if (p) { return p->GetGameObject(); } else { return nullptr; } }
    void SetParent(GameObject *parent) { transform->SetParent(parent->GetTransform()); }
    void SetSibling(GameObject *sibling) { transform->SetSibling(sibling->GetTransform()); }

    template <class T, std::enable_if_t<std::is_base_of_v<Component, T>, bool> = true>
    T *GetComponent() const {
        for (Component *component : components) {
            if (Type::IsBaseOf(T::StaticType(), component->GetType())) { return (T *)component; }
        }
        return nullptr;
    }
    Component *GetComponent(Type *type) const {
        if (!Type::IsBaseOf(Component::StaticType(), type)) {
            return nullptr;
        }
        for (Component *component : components) {
            if (Type::IsBaseOf(type, component->GetType())) { return component; }
        }
        return nullptr;
    }

    template <class T, std::enable_if_t<std::is_base_of_v<Component, T>, bool> = true>
    std::vector<T *> GetComponents() const {
        std::vector<T *> ret;
        for (Component *component : components) {
            if (Type::IsBaseOf(T::StaticType(), component->GetType())) { ret.push_back((T *)component); }
        }
        return ret;
    }
    std::vector<Component *> GetComponents(Type *type) const {
        std::vector<Component *> ret;
        if (!Type::IsBaseOf(Component::StaticType(), type)) {
            return ret;
        }
        for (Component *component : components) {
            if (Type::IsBaseOf(type, component->GetType())) { ret.push_back(component); }
        }
        return ret;
    }

    template <class T, std::enable_if_t<std::is_base_of_v<Component, T> && !std::is_abstract_v<T> && !std::is_same_v<Transform, T>, bool> = true> 
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
    Component *AddComponent(Type *type) {
        if (!Type::IsBaseOf(Component::StaticType(), type) || type->IsAbstract()) {
            return nullptr;
        }
        if (Transform::StaticType() == type) {
            return AddComponent<Transform>();
        } else {
            Component *component = (Component *)type->Instantiate();
            component->gameObject = this;
            components.push_back(component);
            return component;
        }
    }
    template <class T, std::enable_if_t<std::is_base_of_v<Component, T> && !std::is_abstract_v<T> && !std::is_same_v<Transform, T>, bool> = true> 
    T *AddComponent(T *component) {
        nlohmann::json js;
        T::Serialize(js, component);
        T *t = new T();
        T::Deserialize(js, t);
        t->gameObject = this;
        components.push_back(t);
        return t;
    }
    template <class T, std::enable_if_t<std::is_same_v<Transform, T>, bool> = true> 
    T *AddComponent(T *component) {
        // prohibit adding multiple transforms
        if (transform) {
            return transform;
        }
        nlohmann::json js;
        T::Serialize(js, component);
        T *t = new T();
        T::Deserialize(js, t);
        t->gameObject = this;
        components.push_back(t);
        transform = t;
        return t;
    }
    Component *AddComponent(Type *type, Component *component) {
        if (!Type::IsBaseOf(Component::StaticType(), type) || type->IsAbstract()) {
            return nullptr;
        }
        if (Transform::StaticType() == type) {
            return AddComponent<Transform>((Transform *)component);
        } else {
            nlohmann::json js;
            type->Serialize(js, component);
            Component *ret = (Component *)type->Instantiate();
            type->Deserialize(js, ret);
            ret->gameObject = this;
            components.push_back(ret);
            return ret;
        }
    }
    template <class T, std::enable_if_t<std::is_base_of_v<Component, T> && !std::is_abstract_v<T> && !std::is_same_v<Transform, T>, bool> = true> 
    T *AddComponent(const nlohmann::json &js) {
        T *t = new T();
        T::Deserialize(js, t);
        t->gameObject = this;
        components.push_back(t);
        return t;
    }
    template <class T, std::enable_if_t<std::is_same_v<Transform, T>, bool> = true> 
    T *AddComponent(const nlohmann::json &js) {
        // prohibit adding multiple transforms
        if (transform) {
            return transform;
        }
        T *t = new T();
        T::Deserialize(js, t);
        t->gameObject = this;
        components.push_back(t);
        transform = t;
        return t;
    }
    Component *AddComponent(Type *type, const nlohmann::json &js) {
        if (!Type::IsBaseOf(Component::StaticType(), type) || type->IsAbstract()) {
            return nullptr;
        }
        if (Transform::StaticType() == type) {
            return AddComponent<Transform>(js);
        } else {
            Component *component = (Component *)type->Instantiate();
            type->Deserialize(js, component);
            component->gameObject = this;
            components.push_back(component);
            return component;
        }
    }

    template <class T, std::enable_if_t<std::is_base_of_v<Component, T>, bool> = true>
    std::vector<T *> FindComponents() const {
        std::vector<T *> ret;
        std::function<void(const GameObject *)> find = [&, this](const GameObject *gameObject) {
            for (Transform *t : gameObject->GetTransform()->GetChildren()) {
                find(t->GetGameObject());
            }
            for (Component *component : gameObject->GetAllComponents()) {
                if (Type::IsBaseOf(T::StaticType(), component->GetType())) { ret.push_back((T *)component); }
            }
        };
        find(this);
        return ret;
    }
    std::vector<Component *> FindComponents(Type *type) const {
        std::vector<Component *> ret;
        if (!Type::IsBaseOf(Component::StaticType(), type)) {
            return ret;
        }
        std::function<void(const GameObject *)> find = [&, this](const GameObject *gameObject) {
            for (Transform *t : gameObject->GetTransform()->GetChildren()) {
                find(t->GetGameObject());
            }
            for (Component *component : gameObject->GetAllComponents()) {
                if (Type::IsBaseOf(type, component->GetType())) { ret.push_back(component); }
            }
        };
        find(this);
        return ret;
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
