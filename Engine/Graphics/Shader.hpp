#pragma once

#include <string> 
#include <GL/glew.h>

#include <EngineExport.hpp>
#include <Resource.hpp>

namespace Engine {
    class ENGINE_EXPORT Shader : public Resource {
    private:
        GLuint id;
        std::string path;
        std::string shaderType;
        
    public:
        virtual ~Shader() override;
        
        virtual void Init() override;

        const std::string &GetPath() const { return path; }
        const std::string &GetShaderType() const { return shaderType; }

        void SetPath(const std::string &path) { this->path = path; }
        void SetShaderType(const std::string &shaderType) { this->shaderType = shaderType; }

        friend class Material;
    };
}

/*
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
*/