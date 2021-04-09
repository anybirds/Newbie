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
        PROPERTY(bool, useDepthRenderTexture, UseDepthRenderTexture);

    public:
        AFramebuffer();
        virtual std::shared_ptr<Resource> GetResource() override;
    };

    /*
    Framebuffer that can have multiple texture attachments 
    */
    class ENGINE_EXPORT Framebuffer : public Resource {
    private:
        std::shared_ptr<Texture> colorTexture;
        std::shared_ptr<Texture> depthTexture;

        int maxWidth;
        int maxHeight;
        int width;
        int height;
        GLuint fbo;
        GLuint rbo;
        
    public:
        Framebuffer(AFramebuffer *aframebuffer);
        virtual ~Framebuffer();

        virtual void Apply() override;

        int GetMaxWidth() const { return maxWidth; }
        int GetMaxHeight() const { return maxHeight; }
        int GetWidth() const { return width; }
        int GetHeight() const { return height; }
        void SetWidth(int width);
        void SetHeight(int height);
        std::shared_ptr<Texture> GetColorTexture() const { return colorTexture; }
        std::shared_ptr<Texture> GetDepthTexture() const { return depthTexture; }
        bool GetUseDepthRenderTexture() const { AFramebuffer *aframebuffer = (AFramebuffer *)asset; return aframebuffer->GetUseDepthRenderTexture(); }

        friend class Camera;
    };
}