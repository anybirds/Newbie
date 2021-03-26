#pragma once

#include <GL/glew.h>

#include <Asset.hpp>
#include <Resource.hpp>

namespace Engine {

    class ATexture;
    class Texture;

    class ENGINE_EXPORT [[Serialize]] AFramebuffer : [[Serialize]] public Asset {
        TYPE(AFramebuffer)

    private:
        ATexture *colorTexture;
        ATexture *depthTexture;

    public:
        virtual std::shared_ptr<Resource> GetResource() override;

        ATexture *GetColorTexture() const { return colorTexture; }
        ATexture *GetDepthTexture() const { return depthTexture; }
        
        void SetColorTexture() { this->colorTexture = colorTexture; }
        void SetDepthTexture() { this->depthTexture = depthTexture; }
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