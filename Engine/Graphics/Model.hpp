#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Asset.hpp>
#include <Resource.hpp>

NAMESPACE(Engine) {
    CLASS_ATTR(AModel, Asset, ENGINE_EXPORT) {
        TYPE(AModel);
    
        PROPERTY(std::string, path, Path);
    
    public:
        virtual std::shared_ptr<Resource> GetResource() override;
    };

    class ENGINE_EXPORT Model : public Resource {
        PROPERTY_GET(std::string, path, Path);

    private:
        Assimp::Importer *importer;

    public:
        Model(AModel *amodel);
        virtual ~Model();

        virtual void Apply() override;

        friend class Scene;
        friend class Mesh;
    };
}