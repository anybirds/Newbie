#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>

#include <Component.hpp>

class Framebuffer;

CLASS_ATTR(Renderer, Component, ENGINE_EXPORT) {
    TYPE(Renderer);

    PROPERTY_GET(unsigned, order, Order);
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
    Renderer();
    virtual void Render() = 0;

    bool IsOrthographic() const { return orthographic; }
    bool IsPerspective() const { return !orthographic; }
    const glm::mat4 &GetNormalization();

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