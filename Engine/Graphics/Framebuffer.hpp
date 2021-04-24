#pragma once

#include <vector>

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

    class ENGINE_EXPORT Framebuffer : public Resource {
        PROPERTY_GET(bool, useDepthRenderTexture, UseDepthRenderTexture);

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

        GLuint GetId() const { return fbo; }
        int GetMaxWidth() const { return maxWidth; }
        int GetMaxHeight() const { return maxHeight; }
        int GetWidth() const { return width; }
        int GetHeight() const { return height; }
        std::shared_ptr<Texture> GetColorTexture() const { return colorTexture; }
        std::shared_ptr<Texture> GetDepthTexture() const { return depthTexture; }

        void SetWidth(int width);
        void SetHeight(int height);

        std::vector<uint8_t> ReadPixels(int x, int y, int width = 1, int height = 1);

        friend class Camera;
    };
}