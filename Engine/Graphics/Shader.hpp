#pragma once

#include <string> 
#include <GL/glew.h>

#include <Asset.hpp>
#include <Resource.hpp>

NAMESPACE(Engine) {

    CLASS_ATTR(AShader, Asset, ENGINE_EXPORT) {
        TYPE(AShader);

        PROPERTY(std::string, path, Path);
        PROPERTY(unsigned, shaderType, ShaderType);

    public:
        AShader();
        virtual std::shared_ptr<Resource> GetResource() override;
    };

    class ENGINE_EXPORT Shader : public Resource {
    private:
        GLuint id;
        
    public:
        enum { VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER };

        Shader(AShader *ashader);
        virtual ~Shader();

        virtual void Apply() override;

        GLuint GetId() const { return id; }
        const std::string &GetPath() const { AShader *ashader = (AShader *)asset; return ashader->GetPath(); }
        unsigned GetShaderType() const { AShader *ashader = (AShader *)asset; return ashader->GetShaderType(); }
    };
}