#include <Asset.hpp>
#include <Project.hpp>

using json = nlohmann::json;
using namespace Engine;

void Engine::from_json(const json &js, Entity *&entity) {
    uint64_t key = js.get<uint64_t>();
    if (key) {
        entity = reinterpret_cast<Entity *>(Entity::temp.at(key));
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