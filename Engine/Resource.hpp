#pragma once

#include <memory>

#include <Entity.hpp>

namespace Engine {
    /*
    Base class for all the resources like Model, Mesh, Material, Shader, Texture and etc.
    */
    class ENGINE_EXPORT Resource : public Entity {
        
    };

    void to_json(nlohmann::json &js, const std::shared_ptr<Resource> &resource);
    void from_json(nlohmann::json &js, std::shared_ptr<Resource> &resource);
}
