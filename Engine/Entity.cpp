#include <cstdint>

#include <Entity.hpp>

using json = nlohmann::json;
using namespace std;
using namespace Engine;

unordered_map<uint64_t, Entity *> Entity::temp;

namespace Engine {
    void to_json(json &js, const Entity *entity) {
        if (entity) {
            js = reinterpret_cast<uint64_t>(entity);
        } else {
            js = nullptr;
        }
    }

    void from_json(const json &js, Entity *&entity) {
        uint64_t key = js.get<uint64_t>();
        if (key) {
            entity = reinterpret_cast<Entity *>(Entity::temp.at(key));
        } else {
            entity = nullptr;
        }
    }
}
