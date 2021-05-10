#pragma once

#include <Asset.hpp>
#include <Resource.hpp>

class GameObject;

CLASS_FINAL_ATTR(APrefab, Asset, ENGINE_EXPORT) {
    TYPE(APrefab);

    PROPERTY(nlohmann::json, js, Json);

public:
    virtual std::shared_ptr<Resource> GetResource() override;
    nlohmann::json &GetJson() { return js; }
};

class ENGINE_EXPORT Prefab : public Resource {
    PROPERTY(nlohmann::json, js, Json);

public:
    Prefab(APrefab *aprefab);
    virtual void Apply() override;
    
    nlohmann::json &GetJson() { return js; }
};