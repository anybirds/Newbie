#include <Asset.hpp>
#include <Resource.hpp>
#include <Project.hpp>

using json = nlohmann::json;
using namespace std;
using namespace Engine;

void Asset::Apply() {
    if (shared_ptr<Resource> sp = resource.lock()) {
        sp->Apply();
    }
}

void Engine::from_json(const json &js, Entity *&entity) {
    uintptr_t key = js.get<uintptr_t>();
    if (key) {
        auto it = Entity::GetMap().find(key);
        if (it == Entity::GetMap().end()) {
            entity = reinterpret_cast<Entity *>(key * (uintptr_t)!Entity::GetNullify());
        } else {
            entity = reinterpret_cast<Entity *>(it->second);
        }
    } else {
        entity = nullptr;
    }
}

void Engine::from_json(const json &js, Asset *&asset) {
    uint64_t key = js.get<uint64_t>();
    if (key) {
        Project &project = Project::GetInstance();
        asset = reinterpret_cast<Asset *>(project.GetAsset<Asset>(key));
    } else {
        asset = nullptr;
    }
}