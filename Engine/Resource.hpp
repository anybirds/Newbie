#pragma once

#include <memory>
#include <nlohmann/json.hpp>

namespace Engine {

    class Asset;

    /*
    Base class for all the resources like Model, Mesh, Material, Shader, Texture and etc.
    */
    class ENGINE_EXPORT Resource {
    protected:
        Asset *asset; 

    public:
        Resource(Asset *asset) : asset(asset) {}
        
        virtual void Apply() = 0;
    };

    void to_json(nlohmann::json &js, const std::shared_ptr<Resource> &resource);
    void from_json(nlohmann::json &js, std::shared_ptr<Resource> &resource);
}
