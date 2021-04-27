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
        void SetPrefab(GameObject *gameObject);
    };

    class ENGINE_EXPORT Prefab : public Resource {
        PROPERTY(bool, dirty, Dirty);
        PROPERTY(nlohmann::json, js, Json);
        PROPERTY_GET(GameObject *, gameObject, GameObject);

    public:
        Prefab(APrefab *aprefab);
        virtual ~Prefab();
        virtual void Apply() override;
    };
}