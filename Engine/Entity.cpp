#include <Entity.hpp>

using json = nlohmann::json;

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