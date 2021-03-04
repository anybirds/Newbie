#ifndef COMPONENT_H
#define COMPONENT_H

#include <Common/Object.hpp>

#include <engine_global.hpp>

namespace Engine {

    class GameObject;

    SER_DECL(ENGINE_EXPORT, Component)

    /*
    Base class for components that comprise a GameObject.
    */
    class ENGINE_EXPORT Component : public Object {
        TYPE_DECL(Component)

        PROPERTY_GET(GameObject *, GameObject, gameObject)

    public:
        Component(const std::string &name, Type *type = Component::type);
        virtual ~Component() override;
    };
}

typedef typename concat<TYPE_LIST, Engine::Component>::type TypeListComponent;
#undef TYPE_LIST
#define TYPE_LIST TypeListComponent

#endif
