#include <iostream>

#include <Graphics/Model.hpp>

using namespace std;
using namespace Assimp;
using namespace Engine;

shared_ptr<Resource> AModel::GetResource() {
    shared_ptr<Resource> sp = make_shared<Model>(this);
    if (resource.expired()) {
        resource = sp;
    }
    return sp;
}

Model::Model(AModel *amodel) : Resource(amodel) {
    Apply();
}

Model::~Model() {
    delete importer;
}

void Model::Apply() {
    if (importer) {
        delete importer;
    }

    importer = new Assimp::Importer();
    const aiScene *scene = importer->ReadFile(GetPath(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot open file: " << GetPath() << '\n';
        throw exception();
    }
}