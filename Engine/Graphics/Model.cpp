#include <iostream>
#include <fstream>
#include <filesystem>

#include <Graphics/Model.hpp>
#include <Project.hpp>

using namespace std;
using namespace Assimp;

shared_ptr<Resource> AModel::GetResource() {
    shared_ptr<Resource> sp;
    if (!(sp = resource.lock())) {
        try {
            sp = make_shared<Model>(this);
        } catch(...) { }
        resource = sp;
    }
    return sp;
}

Model::Model(AModel *amodel) : Resource(amodel), importer(nullptr) {
    Apply();
}

Model::~Model() {
    delete importer;
}

void Model::Apply() {
    Resource::Apply();
    AModel *amodel = (AModel *)asset;
    path = amodel->GetPath();
    
    if (importer) {
        delete importer;
    }

    string absolute;
    Project &project = Project::GetInstance();
    if (project.IsLoaded()) {
        absolute = Project::GetInstance().GetDirectoy() + "/" + GetPath();
    } else {
        absolute = GetPath();
    }
    importer = new Assimp::Importer();
    const aiScene *scene = importer->ReadFile(
        absolute,
        aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        if (scene) { delete scene; }
        cerr << '[' << __FUNCTION__ << ']' << " cannot open file: " << GetPath() << '\n';
        throw exception();
    }

    cerr << '[' << __FUNCTION__ << ']' << " created Model: " << GetName() << '\n';
}