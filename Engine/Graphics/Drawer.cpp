#include <Scene.hpp>
#include <Graphics/Drawer.hpp>

using namespace Engine;

void Drawer::SetOrder(unsigned order) {
    Scene &scene = Scene::GetInstance();
    scene.drawers.erase(this);
    this->order = order;
    scene.drawers.insert(this);
}