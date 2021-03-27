#pragma once

#include <string> 
#include <GL/glew.h>

#include <Asset.hpp>
#include <Resource.hpp>

namespace Engine {

    class ENGINE_EXPORT [[Serialize]] AShader : public Asset {
        TYPE(AShader)

    private:
        std::string path;
        unsigned shaderType;

    public:
        virtual std::shared_ptr<Resource> GetResource() override;
        const std::string &GetPath() const { return path; }
        unsigned GetShaderType() const { return shaderType; }

        void SetPath(const std::string &path) { this->path = path; }
        void SetShaderType(unsigned shaderType) { this->shaderType = shaderType; }
    };

    class ENGINE_EXPORT Shader : public Resource {
    private:
        GLuint id;
        
    public:
        enum { VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER };

        Shader(AShader *ashader);
        virtual ~Shader();

        virtual void Apply() override;

        const std::string &GetPath() const { AShader *ashader = (AShader *)asset; return ashader->GetPath(); }
        unsigned GetShaderType() const { AShader *ashader = (AShader *)asset; return ashader->GetShaderType(); }

        friend class Material;
    };
}