#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>

#include <Graphics/Renderer.hpp>

namespace Engine {

    class Framebuffer;

    /*
    Featured by its normalization matrix.
    Specify camera properties that affect the normalization matrix.
    */
    class ENGINE_EXPORT [[Serialize]] Camera : public Renderer {
        TYPE(Camera)

    private:
        bool dirty;

        glm::mat4 normalization;

        bool orthographic;
        float fovy;
        float aspectRatio;
        float nr; // near
        float fr; // far
        float size;
        
        std::shared_ptr<Framebuffer> framebuffer;

    public:
        Camera() : dirty(true), fovy(60.0f), aspectRatio(1.0f), nr(0.1f), fr(1000.0f) { }

        bool IsOrthographic() const { return orthographic; }
        bool IsPerspective() const { return !orthographic; }
        float GetFovy() const { return fovy; }
        float GetAspectRatio() const { return aspectRatio; }
        float GetNear() const { return nr; }
        float GetFar() const { return fr; }
        float GetSize() const { return size; }
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
