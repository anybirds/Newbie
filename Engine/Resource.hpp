#pragma once

#include <memory>
#include <nlohmann/json.hpp>

#include <EngineExport.hpp>
#include <Asset.hpp>

CLASS_RESOURCE_ATTR(Resource, Entity, ENGINE_EXPORT) {
    TYPE(Resource);

    PROPERTY_GET_ACCESS(Asset *, asset, Asset, protected, public);

public:
    Resource() : asset(nullptr) {} 
    Resource(Asset *asset) : asset(asset) {}
    
    virtual void Apply() = 0 {}

    uint64_t GetSerial() const { return asset->GetSerial(); }
    const std::string &GetName() const { return asset->GetName(); }

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
