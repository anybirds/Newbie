#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Asset.hpp>
#include <Resource.hpp>

namespace Engine {
    class ENGINE_EXPORT [[Serialize]] AModel : public Asset {
        TYPE(AModel)
    
    private:
        std::string path;
    
    public:
        virtual std::shared_ptr<Resource> GetResource() override;
        const std::string &GetPath() const { return path; };

        void SetPath(const std::string &path) { this->path = path; }
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