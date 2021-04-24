#include "RotateScript.hpp"

using namespace glm;
using namespace Engine;

void RotateScript::Update() {
    float dt = Time::GetInstance().GetDeltaTime();
    Transform *transform = GetTransform();
    transform->RotateAround(vec3(0.0f, 0.0f, 1.0f), rate * dt);

    // blink
    Drawer *drawer = GetGameObject()->GetComponent<Drawer>();
    if ((int)Time::GetInstance().GetTime() % 6 < 3) {
        drawer->SetLocalEnabled(false);
    } else {
        drawer->SetLocalEnabled(true);
    }

    if (copy) {
        Scene &scene = Scene::GetInstance();
        GameObject *square = scene.FindGameObject("square");
        GameObject *child = square->AddGameObject(square);
        child->SetName("child");
        child->GetTransform()->SetLocalPosition(vec3(0.0f, 0.0f, 1.0f));
        GameObject *copyed = scene.AddGameObject(square);
        copyed->GetTransform()->SetLocalPosition(vec3(0.0f, 5.0f, 0.0f));
        copy = false;
    }
}