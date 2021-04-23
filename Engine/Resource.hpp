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
    private:
        uint64_t serial;
        std::string name;
        bool removed;

    protected:
        Asset *asset; 

    public:
        Resource(Asset *asset) : asset(asset) {}
        
        virtual void Apply();

        const std::string &GetName() const { return name; }
        const uint64_t GetSerial() const { return serial; }
        bool IsRemoved() const { return removed; }

        friend class Asset;
    };

    void ENGINE_EXPORT to_json(nlohmann::json &js, const std::shared_ptr<Resource> &resource);
    void ENGINE_EXPORT from_json(const nlohmann::json &js, std::shared_ptr<Resource> &resource);

    template <typename T, std::enable_if_t<std::is_base_of_v<Resource, T>, bool> = true>
    void from_json(const nlohmann::json &js, std::shared_ptr<T> &t) {
        std::shared_ptr<Resource> resource;
        from_json(js, resource);
        t = std::dynamic_pointer_cast<T>(resource);
    }
}
