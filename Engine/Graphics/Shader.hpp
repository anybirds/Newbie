#pragma once

#include <string> 
#include <GL/glew.h>

#include <Asset.hpp>
#include <Resource.hpp>

CLASS_ATTR(AShader, Asset, ENGINE_EXPORT) {
    TYPE(AShader);

    PROPERTY(std::string, path, Path);
    PROPERTY(unsigned, shaderType, ShaderType);

public:
    AShader();
    virtual std::shared_ptr<Resource> GetResource() override;
};

CLASS_RESOURCE_ATTR(Shader, Resource, ENGINE_EXPORT) {
    TYPE(Shader);

    PROPERTY_GET(std::string, path, Path);
    PROPERTY_GET(unsigned, shaderType, ShaderType);
    PROPERTY_GET(GLuint, id, Id);

public:
    using asset_type = AShader;
    enum { VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER };

    Shader(AShader *ashader);
    virtual ~Shader();

    virtual void Apply() override;
};