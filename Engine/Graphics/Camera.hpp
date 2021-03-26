#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>

#include <Component.hpp>
#include <Type.hpp>
#include <IRender.hpp>

namespace Engine {

    class Framebuffer;

    /*
    Featured by its normalization matrix.
    Specify camera properties that affect the normalization matrix.
    */
    class ENGINE_EXPORT [[Serialize]] Camera : [[Serialize]] public Component, public IRender {
        TYPE(Camera)

    private:
        glm::mat4 normalization;

        bool orthographic;
        float fovy;
        float aspectRatio;
        float nr; // near
        float fr; // far
        float size;

        std::shared_ptr<Framebuffer> framebuffer;
    
    public:
        Camera() : fovy(60.0f), aspectRatio(1.0f), nr(0.1f), fr(1000.0f) { ComputeNormalization(); }

        void ComputeNormalization();

        bool IsOrthographic() const { return orthographic; }
        bool IsPerspective() const { return !orthographic; }
        float GetFovy() const { return fovy; }
        float GetAspectRatio() const { return aspectRatio; }
        float GetNear() const { return nr; }
        float GetFar() const { return fr; }
        float GetSize() const { return size; }
        const glm::mat4 &GetNormalization() { return normalization; }

        void SetOrthographic() { orthographic = true; ComputeNormalization(); }
        void SetPerspective() { orthographic = false; ComputeNormalization(); }
        void SetFovy(float fovy);
        void SetNear(float nr);
        void SetFar(float fr);
        void SetSize(float size);

        virtual void Render() override;

        friend class Renderer;
    };
}
