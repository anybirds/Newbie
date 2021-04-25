#include "SampleScript.hpp"

using namespace std;
using namespace glm;
using namespace Engine;

void SampleScript::Start() {
    Scene &scene = Scene::GetInstance();
        
    GameObject *square = scene.FindGameObject("square");
    GameObject *child = square->AddGameObject(square);
    child->SetName("child");
    child->GetTransform()->SetLocalPosition(vec3(0.0f, 0.0f, 1.0f));
    GetTransform()->SetParent(square->GetTransform());

    GameObject *copy = scene.AddGameObject(square);
    copy->GetTransform()->SetLocalPosition(vec3(0.0f, 5.0f, 0.0f));
    copy->FindGameObject("girl")->GetComponent<SampleScript>()->SetLocalEnabled(false);
}

void SampleScript::Update() {

    float dt = Time::GetInstance().GetDeltaTime();
    Transform *transform = GetTransform();
    transform->RotateAround(vec3(0.0f, 0.0f, 1.0f), rate * dt);

    /*
    Drawer *drawer = GetGameObject()->GetComponent<Drawer>();
    if ((int)Time::GetInstance().GetTime() % 6 < 3) {
        drawer->SetLocalEnabled(false);
    } else {
        drawer->SetLocalEnabled(true);
    }
    */
}