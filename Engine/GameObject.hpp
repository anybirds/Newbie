#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <typeinfo>
#include <unordered_map>

#include <Common/Object.hpp>
#include <Common/Transform.hpp>

#include <engine_global.hpp>

namespace Engine {

    class Component;

    SER_DECL(ENGINE_EXPORT, GameObject)

    void ENGINE_EXPORT Destroy(GameObject *go);

    /*
    An atraction of a gameobject in the scene.
    Has components that can describe the gameobject behavior.
    */
    class ENGINE_EXPORT GameObject final : public Object {
        TYPE_DECL(GameObject)

    public:
        typedef std::unordered_map<std::string, Component *> ComponentMap;

    private:
        PROPERTY(Transform *, Transform, transform)
        ComponentMap components;

    public:
        GameObject(const std::string &name, Type *type = GameObject::type);
        virtual ~GameObject() override;

        const ComponentMap &GetComponents() { return components; }

        template <typename T>
        T *GetComponent() const;
        template <typename T>
        T *AddComponent();

        friend class Component;
        friend void Destroy(GameObject *);
    };

    template <typename T>
    T *GameObject::GetComponent() const {

    }

    template <typename T>
    T *GameObject::AddComponent() {

    }
}

typedef typename concat<TYPE_LIST, Engine::GameObject>::type TypeListGameObject;
#undef TYPE_LIST
#define TYPE_LIST TypeListGameObject

#endif
