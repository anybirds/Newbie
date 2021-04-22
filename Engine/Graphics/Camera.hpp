#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>

#include <Graphics/Renderer.hpp>

NAMESPACE(Engine) {

    class Framebuffer;

    /*
    Featured by its normalization matrix.
    Specify camera properties that affect the normalization matrix.
    */
    CLASS_ATTR(Camera, Renderer, ENGINE_EXPORT) {
        TYPE(Camera);

    private:
        static Framebuffer *defaultFramebuffer;
    
    public:
        static void SetDefaultFramebuffer(const std::shared_ptr<Framebuffer> &defaultFramebuffer) {
            Camera::defaultFramebuffer = defaultFramebuffer.get();
        }

    public:
        virtual void Render() override;
    };
}
