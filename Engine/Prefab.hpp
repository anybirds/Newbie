#pragma once

#include <Asset.hpp>
#include <Resource.hpp>

NAMESPACE(Engine) {
    CLASS_FINAL_ATTR(APrefab, Asset, ENGINE_EXPORT) {
        TYPE(APrefab);

        PROPERTY(std::string, path, Path);
    
    public:
        virtual std::shared_ptr<Resource> GetResource() override;
    };

    class Prefab : public Resource {
        PROPERTY_GET(std::string, path, Path);

    public:
        Prefab(APrefab *aprefab);
        virtual void Apply() override;
    };
}