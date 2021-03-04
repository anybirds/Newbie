#include <map>

#include <Common/Project.hpp>
#include <Graphics/Model.hpp>

using namespace std;
using namespace Assimp;
using namespace Engine;

namespace Engine {
    TYPE_DEF(Model)

    SER_DEF(Model, Resource,
    MEMBER_SER | MEMBER_SHOW, std::string, path
    )
}

bool Model::sceneLoad;

Model::Model(const std::string &name, Type *type) : Resource(name, type) {

}

void Model::OnInit() {
    importer = new Assimp::Importer();
    const aiScene *scene = importer->ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
#ifdef DEBUG
        cout << '[' << __FUNCTION__ << ']' << " cannot open file: " << path << '\n';
#endif
        throw exception();
    }

    loaded = true;
    // shouldLoad = false;
}

void Model::OnDestroy() {
    delete importer;
    loaded = false;
}
