#include <glm/gtx/transform.hpp>

#include <Scene.hpp>
#include <Graphics/Renderer.hpp>

using namespace glm;

Renderer::Renderer() : 
    order(0U), dirty(true), orthographic(false), 
    fovy(60.0f), aspectRatio(1.0f), nr(0.1f), fr(1000.0f), size(5.0f) {}

const mat4 &Renderer::GetNormalization() {
    if (dirty) {
        if (orthographic) {
            normalization = ortho(-size * aspectRatio, size * aspectRatio, -size, size, nr, fr);
        } else {
            normalization = perspective(radians(fovy), aspectRatio, nr, fr);
        }
        dirty = false;
    }
    return normalization;
}

void Renderer::SetOrder(unsigned order) {
    Scene &scene = Scene::GetInstance();
    auto it = scene.renderers.find(this->order);
    if (it != scene.renderers.end()) {
        auto &rendset = it->second;
        if (rendset.find(this) != rendset.end()) {
            rendset.erase(this);
            this->order = order;
            rendset.insert(this);
        }
    }
}

void Renderer::OnTrack() {
    Scene &scene = Scene::GetInstance();
    scene.renderers[order].insert(this);
}

void Renderer::OnUntrack() {
    Scene &scene = Scene::GetInstance();
    scene.renderers[order].erase(this);
}