#pragma once

#include <string>
#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <functional>
#include <type_traits>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <nlohmann/json.hpp>

#include <EngineExport.hpp>

/* serialize and deserialize glm types */
namespace glm {
    void ENGINE_EXPORT to_json(nlohmann::json &js, const glm::vec2 &v);
    void ENGINE_EXPORT to_json(nlohmann::json &js, const glm::vec3 &v);
    void ENGINE_EXPORT to_json(nlohmann::json &js, const glm::vec4 &v);
    void ENGINE_EXPORT to_json(nlohmann::json &js, const glm::mat2 &m);
    void ENGINE_EXPORT to_json(nlohmann::json &js, const glm::mat3 &m);
    void ENGINE_EXPORT to_json(nlohmann::json &js, const glm::mat4 &m);
    void ENGINE_EXPORT to_json(nlohmann::json &js, const glm::quat &q);
    void ENGINE_EXPORT from_json(const nlohmann::json &js, glm::vec2 &v);
    void ENGINE_EXPORT from_json(const nlohmann::json &js, glm::vec3 &v);
    void ENGINE_EXPORT from_json(const nlohmann::json &js, glm::vec4 &v);
    void ENGINE_EXPORT from_json(const nlohmann::json &js, glm::mat2 &m);
    void ENGINE_EXPORT from_json(const nlohmann::json &js, glm::mat3 &m);
    void ENGINE_EXPORT from_json(const nlohmann::json &js, glm::mat4 &m);
    void ENGINE_EXPORT from_json(const nlohmann::json &js, glm::quat &q);
}

class Entity;

template <typename T,
std::enable_if_t<std::is_base_of_v<Entity, T> && !std::is_abstract_v<T>, bool> = true>
Entity *instantiate() {
    return (Entity *)new T();
}
template <typename T,
std::enable_if_t<std::is_base_of_v<Entity, T> && std::is_abstract_v<T>, bool> = true>
Entity *instantiate() {
    return nullptr;
}

template <typename T, template<typename...> class R>
struct is_specialization : std::false_type {};

template <template<typename...> class R, typename... Args>
struct is_specialization<R<Args...>, R>: std::true_type {};

template <typename T>
std::function<void(void *)> &GetVisualize() { static std::function<void(void *)> f; return f; }
template <template<typename...> class R>
std::function<void(const std::function<void(void *)> &, void *)> &GetVisualize() { static std::function<void(const std::function<void(void *)> &, void *)> f; return f; }

template <typename T, std::enable_if_t<
std::is_integral<T>::value, bool> = true>
void visualize(void *p) {
    GetVisualize<T>()(p);
}

template <typename T, std::enable_if_t<is_specialization<T, std::vector>::value, bool> = true>
void visualize(void *p) {
    T &v = *(T *)p;
    std::vector<void *> arg;
    for (auto &e : v) {
        arg.push_back((void *)&e);
    }
    GetVisualize<std::vector>()(visualize<typename T::value_type>, (void *)&arg);
}
