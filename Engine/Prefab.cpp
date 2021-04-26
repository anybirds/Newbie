#include <Prefab.hpp>
#include <GameObject.hpp>

using namespace std;
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

Prefab::Prefab(APrefab *aprefab) : Resource(aprefab) {
    Apply();
}

void Prefab::Apply() {
    Resource::Apply();
}