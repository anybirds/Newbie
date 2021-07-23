#pragma once

#include <cstdint>
#include <memory>
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
    ENGINE_EXPORT void to_json(nlohmann::json &js, const glm::vec2 &v);
    ENGINE_EXPORT void to_json(nlohmann::json &js, const glm::vec3 &v);
    ENGINE_EXPORT void to_json(nlohmann::json &js, const glm::vec4 &v);
    ENGINE_EXPORT void to_json(nlohmann::json &js, const glm::mat2 &m);
    ENGINE_EXPORT void to_json(nlohmann::json &js, const glm::mat3 &m);
    ENGINE_EXPORT void to_json(nlohmann::json &js, const glm::mat4 &m);
    ENGINE_EXPORT void to_json(nlohmann::json &js, const glm::quat &q);
    ENGINE_EXPORT void from_json(const nlohmann::json &js, glm::vec2 &v);
    ENGINE_EXPORT void from_json(const nlohmann::json &js, glm::vec3 &v);
    ENGINE_EXPORT void from_json(const nlohmann::json &js, glm::vec4 &v);
    ENGINE_EXPORT void from_json(const nlohmann::json &js, glm::mat2 &m);
    ENGINE_EXPORT void from_json(const nlohmann::json &js, glm::mat3 &m);
    ENGINE_EXPORT void from_json(const nlohmann::json &js, glm::mat4 &m);
    ENGINE_EXPORT void from_json(const nlohmann::json &js, glm::quat &q);
}

class Entity;
class Resource;

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

template <typename T, template<typename...> typename R>
struct is_specialization : std::false_type {};

template <template<typename...> typename R, typename ...Args>
struct is_specialization<R<Args...>, R>: std::true_type {};

ENGINE_EXPORT inline std::function<void(const std::string &, const std::function<void(void *)> &, void *)> &GetVisualizeProperty() { static std::function<void(const std::string &, const std::function<void(void *)> &, void *)> f; return f; }
template <typename T, bool E>
ENGINE_EXPORT inline std::function<void(void *)> &GetVisualize() { static std::function<void(void *)> f; return f; }
template <template<typename...> typename R, bool E>
ENGINE_EXPORT inline std::function<void(const std::vector<std::function<void(void *)>> &, void *)> &GetVisualize() { static std::function<void(const std::vector<std::function<void(void *)>> &, void *)> f; return f; }

template <typename T, bool E = !std::is_const<T>::value, 
std::enable_if_t<std::is_fundamental<T>::value || 
std::is_same<std::remove_cv_t<T>, std::string>::value || 
std::is_same<std::remove_cv_t<T>, glm::vec2>::value || std::is_same<std::remove_cv_t<T>, glm::vec3>::value || std::is_same<std::remove_cv_t<T>, glm::vec4>::value || 
std::is_same<std::remove_cv_t<T>, glm::mat2>::value || std::is_same<std::remove_cv_t<T>, glm::mat3>::value || std::is_same<std::remove_cv_t<T>, glm::mat4>::value ||
std::is_same<std::remove_cv_t<T>, glm::quat>::value, bool> = true>
void visualize(void *p);
template <typename T, bool E = !std::is_const<T>::value, std::enable_if_t<std::is_pointer<T>::value, bool> = true>
void visualize(void *p);
template <typename T, bool E = !std::is_const<T>::value, std::enable_if_t<is_specialization<T, std::shared_ptr>::value, bool> = true>
void visualize(void *p);
template <typename T, bool E = !std::is_const<T>::value, std::enable_if_t<is_specialization<std::remove_cv_t<T>, std::pair>::value, bool> = true>
void visualize(void *p);
template <typename T, bool E = !std::is_const<T>::value, std::enable_if_t<is_specialization<std::remove_cv_t<T>, std::vector>::value, bool> = true>
void visualize(void *p);
template <typename T, bool E = !std::is_const<T>::value, std::enable_if_t<is_specialization<std::remove_cv_t<T>, std::list>::value, bool> = true>
void visualize(void *p);
template <typename T, bool E = !std::is_const<T>::value, std::enable_if_t<is_specialization<std::remove_cv_t<T>, std::set>::value, bool> = true>
void visualize(void *p);
template <typename T, bool E = !std::is_const<T>::value, std::enable_if_t<is_specialization<std::remove_cv_t<T>, std::unordered_set>::value, bool> = true>
void visualize(void *p);
template <typename T, bool E = !std::is_const<T>::value, std::enable_if_t<is_specialization<std::remove_cv_t<T>, std::map>::value, bool> = true>
void visualize(void *p);
template <typename T, bool E = !std::is_const<T>::value, std::enable_if_t<is_specialization<std::remove_cv_t<T>, std::unordered_map>::value, bool> = true>
void visualize(void *p);

template <typename T, bool E, 
std::enable_if_t<std::is_fundamental<T>::value || 
std::is_same<std::remove_cv_t<T>, std::string>::value || 
std::is_same<std::remove_cv_t<T>, glm::vec2>::value || std::is_same<std::remove_cv_t<T>, glm::vec3>::value || std::is_same<std::remove_cv_t<T>, glm::vec4>::value || 
std::is_same<std::remove_cv_t<T>, glm::mat2>::value || std::is_same<std::remove_cv_t<T>, glm::mat3>::value || std::is_same<std::remove_cv_t<T>, glm::mat4>::value ||
std::is_same<std::remove_cv_t<T>, glm::quat>::value, bool>>
void visualize(void *p) {
    GetVisualize<std::remove_cv_t<T>, E>()(p);
}

template <typename T, bool E, std::enable_if_t<std::is_pointer<T>::value, bool>>
void visualize(void *p) {
    GetVisualize<Entity *, E>()(p);
}

template <typename T, bool E, std::enable_if_t<is_specialization<T, std::shared_ptr>::value, bool>>
void visualize(void *p) {
    GetVisualize<std::shared_ptr<Resource>, E>()(p);
}

template <typename T, bool E, std::enable_if_t<is_specialization<std::remove_cv_t<T>, std::pair>::value, bool>>
void visualize(void *p) {
    T &v = *(T *)p;
    std::pair<void *, void *> arg;
    arg.first = (void *)&v.first;
    arg.second = (void *)&v.second;
    GetVisualize<std::pair, E>()(std::vector<std::function<void(void *)>>{visualize<typename T::first_type, E && !std::is_const<typename T::first_type>::value>, visualize<typename T::second_type, E && !std::is_const<typename T::second_type>::value>}, (void *)&arg);
}

template <typename T, bool E, std::enable_if_t<is_specialization<std::remove_cv_t<T>, std::vector>::value, bool>>
void visualize(void *p) {
    T &v = *(T *)p;
    std::vector<void *> arg;
    for (auto &e : v) {
        arg.push_back((void *)&e);
    }
    GetVisualize<std::vector, E>()(std::vector<std::function<void(void *)>>{visualize<typename T::value_type, E && !std::is_const<typename T::value_type>::value>}, (void *)&arg);
}

template <typename T, bool E, std::enable_if_t<is_specialization<std::remove_cv_t<T>, std::list>::value, bool>>
void visualize(void *p) {
    T &v = *(T *)p;
    std::vector<void *> arg;
    for (auto &e : v) {
        arg.push_back((void *)&e);
    }
    GetVisualize<std::list, E>()(std::vector<std::function<void(void *)>>{visualize<typename T::value_type, E && !std::is_const<typename T::value_type>::value>}, (void *)&arg);
}

template <typename T, bool E, std::enable_if_t<is_specialization<std::remove_cv_t<T>, std::set>::value, bool>>
void visualize(void *p) {
    T &v = *(T *)p;
    std::vector<void *> arg;
    for (auto &e : v) {
        arg.push_back((void *)&e);
    }
    GetVisualize<std::set, E>()(std::vector<std::function<void(void *)>>{visualize<typename T::value_type, false>}, (void *)&arg);
}

template <typename T, bool E, std::enable_if_t<is_specialization<std::remove_cv_t<T>, std::unordered_set>::value, bool>>
void visualize(void *p) {
    T &v = *(T *)p;
    std::vector<void *> arg;
    for (auto &e : v) {
        arg.push_back((void *)&e);
    }
    GetVisualize<std::unordered_set, E>()(std::vector<std::function<void(void *)>>{visualize<typename T::value_type, false>}, (void *)&arg);
}

template <typename T, bool E, std::enable_if_t<is_specialization<std::remove_cv_t<T>, std::map>::value, bool>>
void visualize(void *p) {
    T &v = *(T *)p;
    std::vector<void *> arg;
    for (auto &e : v) {
        arg.push_back((void *)&e);
    }
    GetVisualize<std::map, E>()(std::vector<std::function<void(void *)>>{visualize<typename T::value_type, E && !std::is_const<typename T::value_type>::value>}, (void *)&arg);
} 

template <typename T, bool E, std::enable_if_t<is_specialization<std::remove_cv_t<T>, std::unordered_map>::value, bool>>
void visualize(void *p) {
    T &v = *(T *)p;
    std::vector<void *> arg;
    for (auto &e : v) {
        arg.push_back((void *)&e);
    }
    GetVisualize<std::unordered_map, E>()(std::vector<std::function<void(void *)>>{visualize<typename T::value_type, E && !std::is_const<typename T::value_type>::value>}, (void *)&arg);
} 