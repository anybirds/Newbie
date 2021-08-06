#include <Visualize.hpp>

using namespace std;
using namespace glm;
using json = nlohmann::json;

template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<bool, true>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<bool, false>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<uint8_t, true>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<uint8_t, false>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<int8_t, true>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<int8_t, false>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<uint16_t, true>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<uint16_t, false>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<int16_t, true>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<int16_t, false>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<uint32_t, true>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<uint32_t, false>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<int32_t, true>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<int32_t, false>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<uint64_t, true>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<uint64_t, false>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<int64_t, true>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<int64_t, false>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<float, true>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<float, false>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<double, true>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<double, false>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<vec2, true>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<vec2, false>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<vec3, true>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<vec3, false>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<vec4, true>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<vec4, false>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<mat2, true>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<mat2, false>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<mat3, true>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<mat3, false>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<mat4, true>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<mat4, false>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<quat, true>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<quat, false>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<Entity *, true>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<Entity *, false>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<shared_ptr<Resource>, true>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<shared_ptr<Resource>, false>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<enum_type, true>();
template ENGINE_EXPORT inline function<void(void *)> &GetVisualize<enum_type, false>();

template ENGINE_EXPORT inline function<void(const vector<function<void(void *)>> &, void *)> &GetVisualize<pair, true>();
template ENGINE_EXPORT inline function<void(const vector<function<void(void *)>> &, void *)> &GetVisualize<pair, false>();
template ENGINE_EXPORT inline function<void(const vector<function<void(void *)>> &, void *)> &GetVisualize<vector, true>();
template ENGINE_EXPORT inline function<void(const vector<function<void(void *)>> &, void *)> &GetVisualize<vector, false>();
template ENGINE_EXPORT inline function<void(const vector<function<void(void *)>> &, void *)> &GetVisualize<list, true>();
template ENGINE_EXPORT inline function<void(const vector<function<void(void *)>> &, void *)> &GetVisualize<list, false>();
template ENGINE_EXPORT inline function<void(const vector<function<void(void *)>> &, void *)> &GetVisualize<set, true>();
template ENGINE_EXPORT inline function<void(const vector<function<void(void *)>> &, void *)> &GetVisualize<set, false>();
template ENGINE_EXPORT inline function<void(const vector<function<void(void *)>> &, void *)> &GetVisualize<unordered_set, true>();
template ENGINE_EXPORT inline function<void(const vector<function<void(void *)>> &, void *)> &GetVisualize<unordered_set, false>();
template ENGINE_EXPORT inline function<void(const vector<function<void(void *)>> &, void *)> &GetVisualize<map, true>();
template ENGINE_EXPORT inline function<void(const vector<function<void(void *)>> &, void *)> &GetVisualize<map, false>();
template ENGINE_EXPORT inline function<void(const vector<function<void(void *)>> &, void *)> &GetVisualize<unordered_map, true>();
template ENGINE_EXPORT inline function<void(const vector<function<void(void *)>> &, void *)> &GetVisualize<unordered_map, false>();