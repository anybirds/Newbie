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

        PROPERTY_NONE(bool, dirty);
        PROPERTY_NONE(glm::mat4, normalization);
        PROPERTY_NONE(bool, orthographic);
        PROPERTY_GET(float, fovy, Fovy);
        PROPERTY_GET(float, aspectRatio, AspectRatio);
        PROPERTY_GET(float, nr, Near);
        PROPERTY_GET(float, fr, Far);
        PROPERTY_GET(float, size, Size);
        PROPERTY(std::shared_ptr<Framebuffer>, framebuffer, Framebuffer);

    public:
        Camera() : dirty(true), fovy(60.0f), aspectRatio(1.0f), nr(0.1f), fr(1000.0f) { }

        bool IsOrthographic() const { return orthographic; }
        bool IsPerspective() const { return !orthographic; }
        const glm::mat4 &GetNormalization();

        void SetOrthographic() { orthographic = true; dirty = true; }
        void SetPerspective() { orthographic = false; dirty = true; }
        void SetFovy(float fovy) { this->fovy = fovy; dirty = true; }
        void SetNear(float nr) { this->nr = nr; dirty = true; }
        void SetFar(float fr) { this->fr = fr; dirty = true; }
        void SetSize(float size) { this->size = size; dirty = true; }

        virtual void Render() override;
    };
}
