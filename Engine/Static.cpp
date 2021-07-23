#include <Static.hpp>

using namespace std;
using namespace glm;
using nlohmann::json;

namespace glm {
    void to_json(json &js, const glm::vec2 &v) {
        js = json{v.x, v.y};
    }

    void to_json(json &js, const glm::vec3 &v) {
        js = json{v.x, v.y, v.z};
    }

    void to_json(json &js, const glm::vec4 &v) {
        js = json{v.x, v.y, v.z, v.w};
    }

    void to_json(json &js, const glm::mat2 &m) {
        js = json{
        json{m[0][0], m[0][1]},
        json{m[1][0], m[1][1]},
        };
    }

    void to_json(json &js, const glm::mat3 &m) {
        js = json{
        json{m[0][0], m[0][1], m[0][2]},
        json{m[1][0], m[1][1], m[1][2]},
        json{m[2][0], m[2][1], m[2][2]},
        };
    }

    void to_json(json &js, const glm::mat4 &m) {
        js = json{
        json{m[0][0], m[0][1], m[0][2], m[0][3]},
        json{m[1][0], m[1][1], m[1][2], m[1][3]},
        json{m[2][0], m[2][1], m[2][2], m[2][3]},
        json{m[3][0], m[3][1], m[3][2], m[3][3]}
        };
    }

    void to_json(json &js, const glm::quat &q) {
        js = json{q.x, q.y, q.z, q.w};
    }

    void from_json(const json &js, glm::vec2 &v) {
        v = vec2(js[0].get<float>(), js[1].get<float>());
    }

    void from_json(const json &js, glm::vec3 &v) {
        v = vec3(js[0].get<float>(), js[1].get<float>(), js[2].get<float>());
    }

    void from_json(const json &js, glm::vec4 &v) {
        v = vec4(js[0].get<float>(), js[1].get<float>(), js[2].get<float>(), js[3].get<float>());
    }

    void from_json(const json &js, glm::mat2 &m) {
        m = mat2(
        js[0][0].get<float>(), js[0][1].get<float>(),
        js[1][0].get<float>(), js[1][1].get<float>()
        );
    }

    void from_json(const json &js, glm::mat3 &m) {
        m = mat3(
        js[0][0].get<float>(), js[0][1].get<float>(), js[0][2].get<float>(),
        js[1][0].get<float>(), js[1][1].get<float>(), js[1][2].get<float>(),
        js[2][0].get<float>(), js[2][1].get<float>(), js[2][2].get<float>()
        );
    }

    void from_json(const json &js, glm::mat4 &m) {
        m = mat4(
        js[0][0].get<float>(), js[0][1].get<float>(), js[0][2].get<float>(), js[0][3].get<float>(),
        js[1][0].get<float>(), js[1][1].get<float>(), js[1][2].get<float>(), js[1][3].get<float>(),
        js[2][0].get<float>(), js[2][1].get<float>(), js[2][2].get<float>(), js[2][3].get<float>(),
        js[3][0].get<float>(), js[3][1].get<float>(), js[3][2].get<float>(), js[3][3].get<float>()
        );
    }

    void from_json(const json &js, glm::quat &q) {
        q = quat(js[3].get<float>(), js[0].get<float>(), js[1].get<float>(), js[2].get<float>());
    }
}

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