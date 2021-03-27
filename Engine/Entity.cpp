#include <cstdint>

#include <Entity.hpp>

using json = nlohmann::json;
using namespace std;
using namespace Engine;

unordered_map<uint64_t, Entity *> Entity::temp;