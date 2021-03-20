#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>

#include <Component.hpp>
#include <Type.hpp>
#include <IRender.hpp>
#include <Graphics/Window.hpp>

namespace Engine {
    /*
    Featured by its normalization matrix.
    Specify camera properties that affect the normalization matrix.
    */
    class ENGINE_EXPORT [[Serialize]] Camera : public Component, public IRender {
        TYPE(Camera)

    private:
        glm::mat4 normalization;

        bool orthographic;
        float fovy;
        float width;
        float height;
        float nr; // near
        float fr; // far
        float left;
        float right;
        float bottom;
        float top;
    
    public:
        Camera() : fovy(60.0f), 
        width(Window::GetInstance().GetWidth()), height(Window::GetInstance().GetHeight()), 
        nr(0.1f), fr(1000.0f) 
        { ComputeNormalization(); }

        void ComputeNormalization();

        bool IsOrthographic() const { return orthographic; }
        bool IsPerspective() const { return !orthographic; }
        float GetFovy() const { return fovy; }
        float GetWidth() const { return width; }
        float GetNear() const { return nr; }
        float GetFar() const { return fr; }
        float GetLeft() const { return left; }
        float GetRight() const { return bottom; }
        float GetTop() const { return top; }
        const glm::mat4 &GetNormalization() { return normalization; }

        void SetOrthographic() { orthographic = true; ComputeNormalization(); }
        void SetPerspective() { orthographic = false; ComputeNormalization(); }
        void SetFovy(float fovy);
        void SetWidth(float width);
        void SetHeight(float height);
        void SetNear(float nr);
        void SetFar(float fr);
        void SetLeft(float left);
        void SetRight(float right);
        void SetTop(float top);
        void SetBottom(float bottom);

        virtual void Render() override;

        friend class Renderer;
    };
}
