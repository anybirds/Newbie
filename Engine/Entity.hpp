#pragma once

#include <unordered_map>
#include <nlohmann/json.hpp>

#include <EngineExport.hpp>

namespace Engine {
    
    /*
    Base class for all serializable types
    */
    class ENGINE_EXPORT Entity {
    public:
        static std::unordered_map<uint64_t, Entity *> temp;
    };    
    
    void to_json(nlohmann::json &js, const Entity *entity);
    void from_json(nlohmann::json &js, Entity *&entity);
}