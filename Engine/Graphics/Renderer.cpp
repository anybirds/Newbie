#include <Scene.hpp>
#include <Graphics/Renderer.hpp>

using namespace Engine;

void Renderer::SetOrder(unsigned order) {
    Scene &scene = Scene::GetInstance();
    scene.renderers.erase(this);
    this->order = order;
    scene.renderers.insert(this);
}