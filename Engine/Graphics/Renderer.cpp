#include <glm/gtx/transform.hpp>

#include <Scene.hpp>
#include <Graphics/Renderer.hpp>

using namespace glm;
using namespace Engine;

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
    if (scene.renderers.find(this) != scene.renderers.end()) { 
        scene.renderers.erase(this);
        this->order = order;
        scene.renderers.insert(this);   
    }
}

void Renderer::OnEnable() {
    Scene &scene = Scene::GetInstance();
    scene.renderers.insert(this);
}

void Renderer::OnDisable() {
    Scene &scene = Scene::GetInstance();
    scene.renderers.erase(this);
}