#pragma once

#include <vector>

#include <GL/glew.h>

#include <Asset.hpp>
#include <Resource.hpp>

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

CLASS_RESOURCE_ATTR(Framebuffer, Resource, ENGINE_EXPORT) {
    TYPE(Framebuffer);
    
    PROPERTY_GET(std::shared_ptr<Texture>, colorTexture, ColorTexture);
    PROPERTY_GET(std::shared_ptr<Texture>, depthTexture, DepthTexture);
    PROPERTY_GET(bool, useDepthRenderTexture, UseDepthRenderTexture);
    PROPERTY_GET(int, maxWidth, MaxWidth);
    PROPERTY_GET(int, maxHeight, MaxHeight);
    PROPERTY(int, width, Width);
    PROPERTY(int, height, Height);
    PROPERTY_GET(GLuint, fbo, Id);
    PROPERTY_NONE(GLuint, rbo);

public: 
    static void Blit(const std::shared_ptr<Framebuffer> &src, const std::shared_ptr<Framebuffer> &dest, unsigned mask = GL_COLOR_BUFFER_BIT, unsigned filter = GL_NEAREST);
    
public:
    using asset_type = AFramebuffer;
    enum {
        COLOR = GL_COLOR_BUFFER_BIT,
        DEPTH = GL_DEPTH_BUFFER_BIT,
        STENCIL = GL_STENCIL_BUFFER_BIT
    };

    Framebuffer(AFramebuffer *aframebuffer);
    virtual ~Framebuffer();

    virtual void Apply() override;

    void SetWidth(int width);
    void SetHeight(int height);

    void ReadPixels(void *data, int x, int y, int width = 1, int height = 1);

    friend class Camera;
};