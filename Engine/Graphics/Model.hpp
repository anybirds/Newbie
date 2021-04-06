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
    private:
        Assimp::Importer *importer;
    
    public:
        Model(AModel *amodel);
        virtual ~Model();

        virtual void Apply() override;

        const std::string &GetPath() const { AModel *amodel = (AModel *)asset; return amodel->GetPath(); }

        friend class Scene;
        friend class Mesh;
    };
}