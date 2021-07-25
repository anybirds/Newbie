#pragma once

#include <string>

#include <Asset.hpp>
#include <Resource.hpp>

class GameObject;

CLASS_ATTR(APrefab, Asset, ENGINE_EXPORT) {
    TYPE(APrefab);

    PROPERTY(std::string, path, Path);

public:
    virtual std::shared_ptr<Resource> GetResource() override;
};

CLASS_RESOURCE_ATTR(Prefab, Resource, ENGINE_EXPORT) {
    TYPE(Prefab);

    PROPERTY_GET(std::string, path, Path);
    nlohmann::json js;

public:
    using asset_type = APrefab;
    Prefab(APrefab *aprefab);
    virtual void Apply() override;

    const nlohmann::json &GetJson() const { return js; }
};