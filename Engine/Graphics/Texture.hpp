#pragma once

#include <string>
#include <GL/glew.h>

#include <Asset.hpp>
#include <Resource.hpp>

namespace Engine {
    
    class ENGINE_EXPORT [[Serialize]] ATexture : [[Serialize]] public Asset {
        TYPE(ATexture)
    
    private:
        std::string path;

        int width;
        int height;
        unsigned format;
        int horizontalWrap;
        int verticalWrap;
        int minFilter;
        int magFilter;

    public:
        virtual std::shared_ptr<Resource> GetResource() override;
        
        const std::string &GetPath() const { return path; }
        int GetWidth() const { return width; }
        int GetHeight() const { return height; }
        int GetFormat() const { return format; }
        int GetHorizontalWrap() const { return horizontalWrap; }
        int GetVerticalWrap() const { return verticalWrap; }
        int GetMinFilter() const { return minFilter; }
        int GetMagFilter() const { return magFilter; }

        void SetPath(const std::string &path) { this->path = path; }
        void SetWidth(int width) { this->width = width; }
        void SetHeight(int height) { this->height = height; }
        void SetFormat(unsigned format) { this->format = format; }
        void SetHorizontalWrap(int horizontalWrap) { this->horizontalWrap = horizontalWrap; }
        void SetVerticalWrap(int verticalWrap) { this->verticalWrap = verticalWrap; }
        void SetMinFilter(int minFilter) { this->minFilter = minFilter; }
        void SetMagFilter(int magFilter) { this->magFilter = magFilter; }
    };

    /*
	Represents a texture that has been created by loading images.
	*/
    class ENGINE_EXPORT Texture : public Resource {
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

        const std::string &GetPath() const { ATexture *atexture = (ATexture *)asset; return atexture->GetPath(); }
        int GetWidth() const { ATexture *atexture = (ATexture *)asset; return atexture->GetWidth(); }
        int GetHeight() const { ATexture *atexture = (ATexture *)asset; return atexture->GetHeight(); }
        int GetFormat() const { ATexture *atexture = (ATexture *)asset; return atexture->GetFormat(); }
        int GetHorizontalWrap() const { ATexture *atexture = (ATexture *)asset; return atexture->GetHorizontalWrap(); }
        int GetVerticalWrap() const { ATexture *atexture = (ATexture *)asset; return atexture->GetVerticalWrap(); }
        int GetMinFilter() const { ATexture *atexture = (ATexture *)asset; return atexture->GetMinFilter(); }
        int GetMagFilter() const { ATexture *atexture = (ATexture *)asset; return atexture->GetMagFilter(); }

        friend class Material;
        friend class Framebuffer;
    };
}