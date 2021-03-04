#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <Common/Resource.hpp>

#include <engine_global.hpp>

namespace Engine {

    SER_DECL(ENGINE_EXPORT, Shader)

    class ENGINE_EXPORT Shader final : public Resource {
        TYPE_DECL(Shader)

        PROPERTY(std::string, Path, path)
        PROPERTY_GET(std::string, ShaderType, shaderType)

    private:
        GLuint id;

    public:
        Shader(const std::string &name, Type *type = Shader::type);

        virtual void OnInit() override;
        virtual void OnDestroy() override;

        friend class Material;
    };
}

typedef typename concat<TYPE_LIST, Engine::Shader>::type TypeListShader;
#undef TYPE_LIST
#define TYPE_LIST TypeListShader


#endif
