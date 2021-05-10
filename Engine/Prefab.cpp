#include <iostream>
#include <fstream>
#include <filesystem>

#include <Prefab.hpp>
#include <GameObject.hpp>
#include <Scene.hpp>
#include <Project.hpp>

using namespace std;
using json = nlohmann::json;

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

Prefab::Prefab(APrefab *aprefab) : Resource(aprefab) {
    Apply();
}

void Prefab::Apply() {
    Resource::Apply();
    APrefab *aprefab = (APrefab *)asset;
    js = aprefab->GetJson();
}