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
    path = aprefab->GetPath();

    ifstream file;
    string absolute;
    Project &project = Project::GetInstance();
    if (project.IsLoaded()) {
        absolute = Project::GetInstance().GetDirectoy() + "/" + GetPath();
    } else {
        absolute = GetPath();
    }
    file.open(filesystem::u8path(absolute));
    if (!file.is_open()) {
        cerr << '[' << __FUNCTION__ << ']' << " failed to find a prefab: " << GetPath() << '\n';
        throw exception();
    }

    file >> js;
}