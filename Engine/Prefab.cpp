#include <iostream>
#include <fstream>
#include <filesystem>

#include <Prefab.hpp>
#include <GameObject.hpp>
#include <Scene.hpp>
#include <Project.hpp>

using namespace std;
using json = nlohmann::json;
using namespace Engine;

shared_ptr<Resource> APrefab::GetResource() {
    shared_ptr<Resource> sp;
    if (!(sp = resource.lock())) {
        try {
            sp = make_shared<Prefab>(this);
        } catch (...) {}
        resource = sp;
    }
    return sp;
}

void APrefab::SetPrefab(GameObject *gameObject) { 
    gameObject->prefab = dynamic_pointer_cast<Prefab>(GetResource()); 
}

Prefab::Prefab(APrefab *aprefab) : Resource(aprefab), gameObject(nullptr) {
    Apply();
}

Prefab::~Prefab() {
    Scene::GetInstance().DestroyGameObject(gameObject);
}

void Prefab::Apply() {
    Resource::Apply();
    APrefab *aprefab = (APrefab *)asset;
    js = aprefab->GetJson();

    Scene &scene = Scene::GetInstance();
    if (gameObject) {
        scene.DestroyGameObject(gameObject);
    }

    if (js.empty()) {
        return;
    }
    vector<GameObject *> roots;
    GameObject::FromJson(js, roots);
    gameObject = roots[0];
}