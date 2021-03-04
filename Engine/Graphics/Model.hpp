#ifndef MODEL_H
#define MODEL

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Common/Resource.hpp>

#include <engine_global.hpp>

namespace Engine {

    SER_DECL(ENGINE_EXPORT, Model)

    class ENGINE_EXPORT Model final : public Resource {
        TYPE_DECL(Model)

        PROPERTY(std::string, Path, path)

    private:
        static bool sceneLoad;

        Assimp::Importer *importer;

    public:
        Model(const std::string &name, Type *type = Model::type);

        virtual void OnInit() override;
        virtual void OnDestroy() override;

        friend class Scene;
        friend class Mesh;
    };
}

typedef typename concat<TYPE_LIST, Engine::Model>::type TypeListModel;
#undef TYPE_LIST
#define TYPE_LIST TypeListModel

#endif
