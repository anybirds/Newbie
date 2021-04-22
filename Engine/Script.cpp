#include <Script.hpp>
#include <Scene.hpp>

using namespace Engine;

void Script::OnEnable() {
    Scene &scene = Scene::GetInstance();
    scene.scripts.insert(this);
}

void Script::OnDisable() {
    Scene &scene = Scene::GetInstance();
    scene.scripts.erase(this);
}