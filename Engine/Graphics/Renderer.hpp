#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>

#include <Component.hpp>

class Framebuffer;

CLASS_ATTR(Renderer, Component, ENGINE_EXPORT) {
    TYPE(Renderer);

    PROPERTY(unsigned, order, Order);
    PROPERTY_BOOL(bool, orthographic, Orthographic);
    PROPERTY(float, fovy, Fovy);
    PROPERTY_BOOL(bool, aspectRatioFixed, AspectRatioFixed);
    PROPERTY(float, aspectRatio, AspectRatio);
    PROPERTY(float, nr, Near);
    PROPERTY(float, fr, Far);
    PROPERTY(float, size, Size);
    PROPERTY(std::shared_ptr<Framebuffer>, framebuffer, Framebuffer);

private:
    mutable bool dirty;
    mutable glm::mat4 normalization;

public:
    Renderer();
    virtual void Render() = 0;

    const glm::mat4 &GetNormalization() const;

    void SetOrder(unsigned order);
    void SetOrthographic() { orthographic = true; dirty = true; }
    void SetPerspective() { orthographic = false; dirty = true; }
    void SetFovy(float fovy) { this->fovy = fovy; dirty = true; }
    void SetAspectRatio(float aspectRatio) { this->aspectRatio = aspectRatio; dirty = true; }
    void SetNear(float nr) { this->nr = nr; dirty = true; }
    void SetFar(float fr) { this->fr = fr; dirty = true; }
    void SetSize(float size) { this->size = size; dirty = true; }

    virtual void OnTrack() override;
    virtual void OnUntrack() override;
};