#pragma once

#include <Asset.hpp>
#include <Resource.hpp>

NAMESPACE(Engine) {

    class GameObject;

    CLASS_FINAL_ATTR(APrefab, Asset, ENGINE_EXPORT) {
        TYPE(APrefab);

        PROPERTY(nlohmann::json, js, Json);
    
    public:
        virtual std::shared_ptr<Resource> GetResource() override;
        nlohmann::json &GetJson() { return js; }
    };

    class ENGINE_EXPORT Prefab : public Resource {
        PROPERTY(bool, dirty, Dirty);
        PROPERTY(nlohmann::json, js, Json);
        PROPERTY_GET(GameObject *, gameObject, GameObject);
        PROPERTY_GET(uintptr_t, entityCount, EntityCount);

    public:
        Prefab(APrefab *aprefab);
        virtual ~Prefab();
        virtual void Apply() override;
        
        nlohmann::json &GetJson() { return js; }
    };
}