#pragma once

#include <memory>
#include <nlohmann/json.hpp>

#include <EngineExport.hpp>
#include <Asset.hpp>

namespace Engine {

    /*
    Base class for all the resources like Model, Mesh, Material, Shader, Texture and etc.
    */
    class ENGINE_EXPORT Resource {
    protected:
        Asset *asset; 

    public:
        Resource(Asset *asset) : asset(asset) {}
        
        virtual void Apply() = 0;

        const std::string &GetName() const { return asset->GetName(); }
        void SetName(const std::string &name) { asset->SetName(name); }
    };

    void to_json(nlohmann::json &js, const std::shared_ptr<Resource> &resource);
    void from_json(const nlohmann::json &js, std::shared_ptr<Resource> &resource);

    template <typename T, std::enable_if_t<std::is_base_of_v<Resource, T>, bool> = true>
    void from_json(const nlohmann::json &js, std::shared_ptr<T> &t) {
        std::shared_ptr<Resource> resource;
        from_json(js, resource);
        t = dynamic_pointer_cast<T>(resource);
    }
}
