#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Asset.hpp>
#include <Resource.hpp>

CLASS_ATTR(AModel, Asset, ENGINE_EXPORT) {
    TYPE(AModel);

    PROPERTY(std::string, path, Path);

public:
    virtual std::shared_ptr<Resource> GetResource() override;
};

CLASS_RESOURCE_ATTR(Model, Resource, ENGINE_EXPORT) {
    TYPE(Model);

    PROPERTY_GET(std::string, path, Path);

private:
    Assimp::Importer *importer;

public:
    using asset_type = AModel;
    Model(AModel *amodel);
    virtual ~Model();

    virtual void Apply() override;

    friend class Mesh;
};