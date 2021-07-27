#include <Asset.hpp>
#include <Resource.hpp>
#include <Project.hpp>

using json = nlohmann::json;
using namespace std;

void Asset::Apply() {
    if (shared_ptr<Resource> sp = resource.lock()) {
        try {
            sp->Apply();
        } catch (...) {}
    }
}

void Asset::Remove() {
    Project::GetInstance().assets.erase(serial);
}

bool Asset::IsRemoved() {
    auto &assets = Project::GetInstance().GetAllAssets();
    return assets.find(serial) == assets.end();
}

void from_json(const json &js, Entity *&entity) {
    uintptr_t key = js.get<uintptr_t>();
    if (key) {
        auto it = Entity::GetMap().find(key);
        if (it == Entity::GetMap().end()) {
            if (Entity::GetNullify()) {
                entity = nullptr;
            } else {
                entity = (Entity *)key;
            }
        } else {
            entity = (Entity *)it->second;
        }
    } else {
        entity = nullptr;
    }
}

void from_json(const json &js, Asset *&asset) {
    uint64_t key = js.get<uint64_t>();
    if (key) {
        Project &project = Project::GetInstance();
        asset = project.GetAsset(key);
    } else {
        asset = nullptr;
    }
}