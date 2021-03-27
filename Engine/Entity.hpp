#pragma once

#include <unordered_map>
#include <nlohmann/json.hpp>

#include <Type.hpp>

namespace Engine {
    
    class Type;
    class Interface;

    /*
    Base class for all serializable types
    */
    class ENGINE_EXPORT [[Serialize]] Entity {
        TYPE(Entity)

    public:
        static std::unordered_map<uint64_t, Entity *> temp;

    public:
        virtual ~Entity() {}
    };
}