#include <Script.hpp>
#include <Scene.hpp>

void Script::OnTrack() {
    Scene &scene = Scene::GetInstance();
    scene.scripts.insert(this);
}

void Script::OnUntrack() {
    Scene &scene = Scene::GetInstance();
    scene.scripts.erase(this);
}