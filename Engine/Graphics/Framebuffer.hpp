#pragma once

#include <GL/glew.h>

#include <Asset.hpp>
#include <Resource.hpp>

NAMESPACE(Engine) {

    class ATexture;
    class Texture;

    CLASS_ATTR(AFramebuffer, Asset, ENGINE_EXPORT) {
        TYPE(AFramebuffer);

        PROPERTY(ATexture *, colorTexture, ColorTexture);
        PROPERTY(ATexture *, depthTexture, DepthTexture);

    public:
        virtual std::shared_ptr<Resource> GetResource() override;
    };

    /*
    Framebuffer that can have multiple texture attachments 
    */
    class ENGINE_EXPORT Framebuffer : public Resource {
    private:
        std::shared_ptr<Texture> colorTexture;
        std::shared_ptr<Texture> depthTexture;

        int width;
        int height;
        GLuint fbo;
        
    public:
        Framebuffer(AFramebuffer *aframebuffer);
        virtual ~Framebuffer();

        virtual void Apply() override;

        int GetWidth() const { return width; }
        int GetHeight() const { return height; }
        std::shared_ptr<Texture> GetColorTexture() const { return colorTexture; }
        std::shared_ptr<Texture> GetDepthTexture() const { return depthTexture; }

        friend class Camera;
    };
}