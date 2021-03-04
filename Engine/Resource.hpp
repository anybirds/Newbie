#ifndef RESOURCE_H
#define RESOURCE_H

#include <Common/Object.hpp>

#include <engine_global.hpp>

namespace Engine {

    SER_DECL(ENGINE_EXPORT, Resource)

    /*
    Base class for all the resources like Model, Mesh, Material, Shader, Texture and etc.
    */
    class ENGINE_EXPORT Resource : public Object {
        TYPE_DECL(Resource)

    private:
        static bool sceneLoad;

    protected:
        bool loaded;
        bool shouldLoad;

    public:
        Resource(const std::string &name, Type *type = Resource::type);
        virtual ~Resource() override;

        virtual void OnInit() override { loaded = true; shouldLoad = true; }
        virtual void OnDestroy() override { loaded = false; }

        friend class Scene;
        friend void from_json(const json &, Object *&);
    };
}

typedef typename concat<TYPE_LIST, Engine::Resource>::type TypeListResource;
#undef TYPE_LIST
#define TYPE_LIST TypeListResource

#endif
