#pragma once

#include <string>

#include <Asset.hpp>
#include <Resource.hpp>

class GameObject;

CLASS_FINAL_ATTR(APrefab, Asset, ENGINE_EXPORT) {
    TYPE(APrefab);

    PROPERTY(std::string, path, Path);

public:
    virtual std::shared_ptr<Resource> GetResource() override;
};

class ENGINE_EXPORT Prefab : public Resource {
    PROPERTY_GET(std::string, path, Path);
    PROPERTY_GET(nlohmann::json, js, Json);

public:
    Prefab(APrefab *aprefab);
    virtual void Apply() override;
};