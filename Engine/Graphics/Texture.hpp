#pragma once

#include <string>
#include <GL/glew.h>

#include <Asset.hpp>
#include <Resource.hpp>

CLASS_ATTR(ATexture, Asset, ENGINE_EXPORT) {
    TYPE(ATexture);

    PROPERTY(std::string, path, Path);
    PROPERTY(int, width, Width);
    PROPERTY(int, height, Height);
    PROPERTY(unsigned, internalFormat, InternalFormat);
    PROPERTY(unsigned, format, Format);
    PROPERTY(unsigned, dataType, DataType);
    PROPERTY(int, horizontalWrap, HorizontalWrap);
    PROPERTY(int, verticalWrap, VerticalWrap);
    PROPERTY(int, minFilter, MinFilter);
    PROPERTY(int, magFilter, MagFilter);

public:
    ATexture();
    virtual std::shared_ptr<Resource> GetResource() override;
};

CLASS_RESOURCE_ATTR(Texture, Resource, ENGINE_EXPORT) {
    TYPE(Texture);

    PROPERTY_GET(std::string, path, Path);
    PROPERTY_GET(int, width, Width);
    PROPERTY_GET(int, height, Height);
    PROPERTY_GET(unsigned, internalFormat, InternalFormat);
    PROPERTY_GET(unsigned, format, Format);
    PROPERTY_GET(unsigned, dataType, DataType);
    PROPERTY_GET(int, horizontalWrap, HorizontalWrap);
    PROPERTY_GET(int, verticalWrap, VerticalWrap);
    PROPERTY_GET(int, minFilter, MinFilter);
    PROPERTY_GET(int, magFilter, MagFilter);
    PROPERTY_GET(GLuint, id, Id);

public:
    using asset_type = ATexture;
    enum { 
        NEAREST = GL_NEAREST, LINEAR = GL_LINEAR, 
        CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE, CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER, 
        MIRRORED_REPEAT = GL_MIRRORED_REPEAT, REPEAT = GL_REPEAT, MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE,
        R32I = GL_R32I,
        RGB = GL_RGB, RGBA = GL_RGBA, RED = GL_RED, RED_INTEGER = GL_RED_INTEGER,
        UNSIGNED_BYTE = GL_UNSIGNED_BYTE, INT = GL_INT
    };

    Texture(ATexture *atexture);
    virtual ~Texture();
    
    virtual void Apply() override;
};