#pragma once

#include <vector>

#include <EngineExport.hpp>
#include <Entity.hpp>
#include <Renderer.hpp>
#include <Script.hpp>

namespace Engine {
    class ENGINE_EXPORT Group final {
    private:
        std::vector<Entity *> entities;
        std::vector<Renderer *> renderers;
        std::vector<Script *> scripts;

    public:
        Group();
        ~Group();

        void Insert(Entity *entity) { entities.push_back(entity); }
        void Insert(Renderer *renderer) { renderers.push_back(renderer); Insert(static_cast<Entity *>(renderer)); }
        void Insert(Script *script) { scripts.push_back(script); Insert(static_cast<Entity *>(script)); }
    };
}

/*
#ifndef COMPONENT_H
#define COMPONENT_H

#include <Common/Object.hpp>

#include <engine_global.hpp>

namespace Engine {

    class GameObject;

    SER_DECL(ENGINE_EXPORT, Component)

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
*/