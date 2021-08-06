#pragma once

#include <string>
#include <GL/glew.h>

#include <Asset.hpp>
#include <Resource.hpp>

ENUM(TextureDataType, unsigned) {
    UNSIGNED_BYTE = GL_UNSIGNED_BYTE, 
    INT = GL_INT,
};

ENUM(TextureFormat, unsigned) {
    R32I = GL_R32I,
    RGB = GL_RGB, 
    RGBA = GL_RGBA, 
    RED = GL_RED, 
    RED_INTEGER = GL_RED_INTEGER,
};

ENUM(TextureWrapMode, int) {
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE, 
    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER, 
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT, 
    REPEAT = GL_REPEAT, 
    MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE,
};

ENUM(TextureFilterMode, int) {
    NEAREST = GL_NEAREST, 
    LINEAR = GL_LINEAR,
};

CLASS_ATTR(ATexture, Asset, ENGINE_EXPORT) {
    TYPE(ATexture);

    PROPERTY(std::string, path, Path);
    PROPERTY(int, width, Width);
    PROPERTY(int, height, Height);
    PROPERTY(TextureFormat, internalFormat, InternalFormat);
    PROPERTY(TextureFormat, format, Format);
    PROPERTY(TextureDataType, dataType, DataType);
    PROPERTY(TextureWrapMode, horizontalWrap, HorizontalWrap);
    PROPERTY(TextureWrapMode, verticalWrap, VerticalWrap);
    PROPERTY(TextureFilterMode, minFilter, MinFilter);
    PROPERTY(TextureFilterMode, magFilter, MagFilter);

public:
    ATexture();
    virtual std::shared_ptr<Resource> GetResource() override;
};

CLASS_RESOURCE_ATTR(Texture, Resource, ENGINE_EXPORT) {
    TYPE(Texture);

    PROPERTY_GET(std::string, path, Path);
    PROPERTY_GET(int, width, Width);
    PROPERTY_GET(int, height, Height);
    PROPERTY_GET(TextureFormat, internalFormat, InternalFormat);
    PROPERTY_GET(TextureFormat, format, Format);
    PROPERTY_GET(TextureDataType, dataType, DataType);
    PROPERTY_GET(TextureWrapMode, horizontalWrap, HorizontalWrap);
    PROPERTY_GET(TextureWrapMode, verticalWrap, VerticalWrap);
    PROPERTY_GET(TextureFilterMode, minFilter, MinFilter);
    PROPERTY_GET(TextureFilterMode, magFilter, MagFilter);
    PROPERTY_GET(GLuint, id, Id);

public:
    using asset_type = ATexture;

    Texture(ATexture *atexture);
    virtual ~Texture();
    
    virtual void Apply() override;
};