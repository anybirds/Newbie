#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>

#include <Graphics/Renderer.hpp>

class Framebuffer;

CLASS_ATTR(Camera, Renderer, ENGINE_EXPORT) {
    TYPE(Camera);

public:
    static Framebuffer *&GetDefaultFramebuffer() {
        static Framebuffer *defaultFramebuffer;
        return defaultFramebuffer;
    }
    static void SetDefaultFramebuffer(const std::shared_ptr<Framebuffer> &defaultFramebuffer) {
        GetDefaultFramebuffer() = defaultFramebuffer.get();
    }

public:
    virtual void Render() override;
};
