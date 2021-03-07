#pragma once

#include <memory>

#include <EngineExport.hpp>
#include <Entity.hpp>

namespace Engine {

    class Resource;

    class ENGINE_EXPORT Asset : public Entity {
    private:
        uint64_t serial;
        std::weak_ptr<Resource> resource;
    
    public:
        std::weak_ptr<Resource> &GetResource() { return resource; }
    };

    void to_json(nlohmann::json &js, const Asset *asset);
    void from_json(nlohmann::json &js, Asset *&asset);   
}