#pragma once

#include <unordered_map>

#include <Type.hpp>

NAMESPACE(Engine) {
    
    /*
    Base class for all serializable types
    */
    CLASS_ENTITY(Entity, , ENGINE_EXPORT) {
        TYPE(Entity);

    public:
        static std::unordered_map<uint64_t, Entity *> temp;

    public:
        virtual ~Entity() {}
    };
}