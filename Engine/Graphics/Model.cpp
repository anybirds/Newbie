#include <iostream>

#include <Graphics/Model.hpp>

using namespace std;
using namespace Assimp;
using namespace Engine;

Model::~Model() {
    delete importer;
    /*
    loaded = false;
    */
}

void Model::Init() {
    importer = new Assimp::Importer();
    const aiScene *scene = importer->ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cerr << '[' << __FUNCTION__ << ']' << " cannot open file: " << path << '\n';
        throw exception();
    }

    /*
    loaded = true;
    // shouldLoad = false;
    */
}