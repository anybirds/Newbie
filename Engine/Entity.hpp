#pragma once

#include <unordered_map>
#include <nlohmann/json.hpp>

#include <EngineExport.hpp>

namespace Engine {
    
    class Type;
    class Interface;

    /*
    Base class for all serializable types
    */
    class ENGINE_EXPORT Entity {
    public:
        static std::unordered_map<uint64_t, Entity *> temp;

    private:
        bool removed;

    public:
        Entity() : removed(false) {}
        virtual ~Entity() {}
        
        bool IsRemoved() const { return removed; }
        virtual void SetRemoved() { removed = true; }
        virtual Type *GetType() const = 0;
    };
}