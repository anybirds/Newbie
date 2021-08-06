#pragma once

#include <string> 
#include <GL/glew.h>

#include <Asset.hpp>
#include <Resource.hpp>

ENUM(ShaderType, unsigned) {
    VERTEX_SHADER = GL_VERTEX_SHADER,
    FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
};

CLASS_ATTR(AShader, Asset, ENGINE_EXPORT) {
    TYPE(AShader);

    PROPERTY(std::string, path, Path);
    PROPERTY(ShaderType, shaderType, ShaderType);

public:
    AShader();
    virtual std::shared_ptr<Resource> GetResource() override;
};

CLASS_RESOURCE_ATTR(Shader, Resource, ENGINE_EXPORT) {
    TYPE(Shader);

    PROPERTY_GET(std::string, path, Path);
    PROPERTY_GET(ShaderType, shaderType, ShaderType);
    PROPERTY_GET(GLuint, id, Id);

public:
    using asset_type = AShader;

    Shader(AShader *ashader);
    virtual ~Shader();

    virtual void Apply() override;
};