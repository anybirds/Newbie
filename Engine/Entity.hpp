#pragma once

#include <unordered_map>
#include <nlohmann/json.hpp>

#include <EngineExport.hpp>

namespace Engine {
    
    class Type;

    /*
    Base class for all serializable types
    */
    class ENGINE_EXPORT Entity {
    public:
        static std::unordered_map<uint64_t, Entity *> temp;

    private:
        bool removed;

    public:
        bool IsRemoved() const { return removed; }
        virtual void SetRemoved() { removed = true; }
        virtual Type *GetType() const = 0;
    };
    
    void to_json(nlohmann::json &js, const Entity *entity);
    void from_json(const nlohmann::json &js, Entity *&entity);
}