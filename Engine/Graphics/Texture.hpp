#pragma once

#include <string>
#include <GL/glew.h>

#include <Asset.hpp>
#include <Resource.hpp>

NAMESPACE(Engine) {
    
    CLASS_ATTR(ATexture, Asset, ENGINE_EXPORT) {
        TYPE(ATexture);
    
        PROPERTY(std::string, path, Path);
        PROPERTY(int, width, Width);
        PROPERTY(int, height, Height);
        PROPERTY(unsigned, format, Format);
        PROPERTY(int, horizontalWrap, HorizontalWrap);
        PROPERTY(int, verticalWrap, VerticalWrap);
        PROPERTY(int, minFilter, MinFilter);
        PROPERTY(int, magFilter, MagFilter);

    public:
        ATexture();
        virtual std::shared_ptr<Resource> GetResource() override;
    };

    /*
	Represents a texture that has been created by loading images.
	*/
    class ENGINE_EXPORT Texture : public Resource {
        PROPERTY_GET(std::string, path, Path);
        PROPERTY_GET(int, width, Width);
        PROPERTY_GET(int, height, Height);
        PROPERTY_GET(unsigned, format, Format);
        PROPERTY_GET(int, horizontalWrap, HorizontalWrap);
        PROPERTY_GET(int, verticalWrap, VerticalWrap);
        PROPERTY_GET(int, minFilter, MinFilter);
        PROPERTY_GET(int, magFilter, MagFilter);
        
    private:
        GLuint id;

    public:
        enum { 
            NEAREST = GL_NEAREST, LINEAR = GL_LINEAR, 
            CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE, CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER, 
            MIRRORED_REPEAT = GL_MIRRORED_REPEAT, REPEAT = GL_REPEAT, MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE,
            RGB = GL_RGB, RGBA = GL_RGBA
        };

        Texture(ATexture *atexture);
        virtual ~Texture();
        
        virtual void Apply() override;

        int GetId() const { return id; }
    };
}