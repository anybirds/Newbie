#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <Entity.hpp>
#include <Type.hpp>

NAMESPACE(Engine) {

    class Resource;

    /*
    Menifest to how to instantiate the Resource object.
    */
    CLASS_ATTR(Asset, Entity, ENGINE_EXPORT) {
        TYPE(Asset);

        PROPERTY_GET(uint64_t, serial, Serial);
        PROPERTY(std::string, name, Name);
        
    protected:
        std::weak_ptr<Resource> resource;

    public:
        void Apply();
        virtual std::shared_ptr<Resource> GetResource() = 0;
        
        friend class Project;
    };

    void ENGINE_EXPORT from_json(const nlohmann::json &js, Entity *&entity);
    void ENGINE_EXPORT from_json(const nlohmann::json &js, Asset *&asset);

    template <typename T>
    void to_json(nlohmann::json &js, const T *t) {
        if (std::is_base_of_v<Entity, T>) {
            js = reinterpret_cast<uint64_t>(t);
        } else {
            js = reinterpret_cast<uint64_t>(dynamic_cast<const Entity *>(t));
        }
    }   

    template <typename T>
    void from_json(const nlohmann::json &js, T *&t) {
        if (std::is_base_of_v<Asset, T>) {
            Asset *asset = nullptr;
            from_json(js, asset);
            t = dynamic_cast<T *>(asset);
        } else {
            Entity *entity = nullptr;
            from_json(js, entity);
            t = dynamic_cast<T *>(entity);
        }
    }
}