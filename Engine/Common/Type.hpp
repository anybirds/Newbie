#ifndef TYPE_H
#define TYPE_H

#include <iostream>
#include <exception>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include <engine_global.hpp>

#define GET(T, P, M) T const &Get##P() const { return M; }
#define SET(T, P, M) void Set##P(T const &M) { this->M = M; }
#define PROPERTY(T, P, M) public: GET(T, P, M) SET(T, P, M) private: T M;
#define PROPERTY_GET(T, P, M) public: GET(T, P, M) private: T M;
#define PROPERTY_SET(T, P, M) public: SET(T, P, M) private: T M;

#define TYPE_DEF(T) Engine::Type *T::type; std::string T::typeName(#T);

#define TYPE_DECL(T) \
    friend class Type; \
    friend void to_json(json &, const T &); \
    friend void from_json(const json &, T &); \
    public: \
    static Type *type; \
    static std::string typeName; \
    static void Instantiate(const std::string &name) { new T(name); } \
    static void Serialize(json &js, const Object *obj) { const T *t = dynamic_cast<const T *>(obj); js = *t; } \
    static void Deserialize(const json &js, Object *obj) { T *t = dynamic_cast<T *>(obj); from_json(js, *t); } \
    private:

#define MEMBER_SER 0x1
#define MEMBER_SHOW 0x10

#define TOJSON_DECL(E, T) class T; void E to_json(json &js, const T &t); void ENGINE_EXPORT to_json(json &js, const T *t);
#define FROMJSON_DECL(E, T) class T; void E from_json(const json &js, T &t); void ENGINE_EXPORT from_json(const json &js, T *&t);

#define TOJSON_BEGIN(T) \
    void to_json(json &js, const T *t) { to_json(js, static_cast<const Object *>(t)); } \
    void to_json(json &js, const T &t) {
#define TOJSON_BASE(B) const B &b = t; js = b;
#define TOJSON_MEMBER(C, T, M) if (C & MEMBER_SER) { js[#M] = t.M; }
#define TOJSON_END }

#define FROMJSON_BEGIN(T) \
    void from_json(const json &js, T *&t) { Object *obj = nullptr; from_json(js, obj); t = dynamic_cast<T *>(obj); } \
    void from_json(const json &js, T &t) {
#define FROMJSON_BASE(B) B &b = t; from_json(js, b);
#define FROMJSON_MEMBER(C, T, M) if (C & MEMBER_SER) { t.M = js[#M].get<T>(); }
#define FROMJSON_END }

#define EXPAND(X) X
#define GET_MACRO(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, NAME, ...) NAME

#define TOJSON_MEMBER_0()
#define TOJSON_MEMBER_1(C, T, M) TOJSON_MEMBER(C, T, M)
#define TOJSON_MEMBER_2(C, T, M, ...) EXPAND(TOJSON_MEMBER(C, T, M) TOJSON_MEMBER_1(__VA_ARGS__))
#define TOJSON_MEMBER_3(C, T, M, ...) EXPAND(TOJSON_MEMBER(C, T, M) TOJSON_MEMBER_2(__VA_ARGS__))
#define TOJSON_MEMBER_4(C, T, M, ...) EXPAND(TOJSON_MEMBER(C, T, M) TOJSON_MEMBER_3(__VA_ARGS__))
#define TOJSON_MEMBER_5(C, T, M, ...) EXPAND(TOJSON_MEMBER(C, T, M) TOJSON_MEMBER_4(__VA_ARGS__))
#define TOJSON_MEMBER_6(C, T, M, ...) EXPAND(TOJSON_MEMBER(C, T, M) TOJSON_MEMBER_5(__VA_ARGS__))
#define TOJSON_MEMBER_7(C, T, M, ...) EXPAND(TOJSON_MEMBER(C, T, M) TOJSON_MEMBER_6(__VA_ARGS__))
#define TOJSON_MEMBER_8(C, T, M, ...) EXPAND(TOJSON_MEMBER(C, T, M) TOJSON_MEMBER_7(__VA_ARGS__))
#define TOJSON_MEMBER_9(C, T, M, ...) EXPAND(TOJSON_MEMBER(C, T, M) TOJSON_MEMBER_8(__VA_ARGS__))
#define TOJSON_MEMBER_10(C, T, M, ...) EXPAND(TOJSON_MEMBER(C, T, M) TOJSON_MEMBER_9(__VA_ARGS__))

#define TOJSON_MEMBER_N(...) \
EXPAND(GET_MACRO(_0, __VA_ARGS__, TOJSON_MEMBER_10, ERROR, ERROR, TOJSON_MEMBER_9, ERROR, ERROR, TOJSON_MEMBER_8, ERROR, ERROR, TOJSON_MEMBER_7, ERROR, ERROR, TOJSON_MEMBER_6, ERROR, ERROR, TOJSON_MEMBER_5, ERROR, ERROR, TOJSON_MEMBER_4, ERROR, ERROR, TOJSON_MEMBER_3, ERROR, ERROR, TOJSON_MEMBER_2, ERROR, ERROR, TOJSON_MEMBER_1, ERROR, ERROR, TOJSON_MEMBER_0)(__VA_ARGS__))

#define FROMJSON_MEMBER_0()
#define FROMJSON_MEMBER_1(C, T, M) FROMJSON_MEMBER(C, T, M)
#define FROMJSON_MEMBER_2(C, T, M, ...) EXPAND(FROMJSON_MEMBER(C, T, M) FROMJSON_MEMBER_1(__VA_ARGS__))
#define FROMJSON_MEMBER_3(C, T, M, ...) EXPAND(FROMJSON_MEMBER(C, T, M) FROMJSON_MEMBER_2(__VA_ARGS__))
#define FROMJSON_MEMBER_4(C, T, M, ...) EXPAND(FROMJSON_MEMBER(C, T, M) FROMJSON_MEMBER_3(__VA_ARGS__))
#define FROMJSON_MEMBER_5(C, T, M, ...) EXPAND(FROMJSON_MEMBER(C, T, M) FROMJSON_MEMBER_4(__VA_ARGS__))
#define FROMJSON_MEMBER_6(C, T, M, ...) EXPAND(FROMJSON_MEMBER(C, T, M) FROMJSON_MEMBER_5(__VA_ARGS__))
#define FROMJSON_MEMBER_7(C, T, M, ...) EXPAND(FROMJSON_MEMBER(C, T, M) FROMJSON_MEMBER_6(__VA_ARGS__))
#define FROMJSON_MEMBER_8(C, T, M, ...) EXPAND(FROMJSON_MEMBER(C, T, M) FROMJSON_MEMBER_7(__VA_ARGS__))
#define FROMJSON_MEMBER_9(C, T, M, ...) EXPAND(FROMJSON_MEMBER(C, T, M) FROMJSON_MEMBER_8(__VA_ARGS__))
#define FROMJSON_MEMBER_10(C, T, M, ...) EXPAND(FROMJSON_MEMBER(C, T, M) FROMJSON_MEMBER_9(__VA_ARGS__))

#define FROMJSON_MEMBER_N(...) \
EXPAND(GET_MACRO(_0, __VA_ARGS__, FROMJSON_MEMBER_10, ERROR, ERROR, FROMJSON_MEMBER_9, ERROR, ERROR, FROMJSON_MEMBER_8, ERROR, ERROR, FROMJSON_MEMBER_7, ERROR, ERROR, FROMJSON_MEMBER_6, ERROR, ERROR, FROMJSON_MEMBER_5, ERROR, ERROR, FROMJSON_MEMBER_4, ERROR, ERROR, FROMJSON_MEMBER_3, ERROR, ERROR, FROMJSON_MEMBER_2, ERROR, ERROR, FROMJSON_MEMBER_1, ERROR, ERROR, FROMJSON_MEMBER_0)(__VA_ARGS__))

#define SER_DECL(E, T) TOJSON_DECL(E, T) FROMJSON_DECL(E, T)
#define SER_DEF(T, B, ...) EXPAND(TOJSON_BEGIN(T) TOJSON_BASE(B) TOJSON_MEMBER_N(__VA_ARGS__) TOJSON_END FROMJSON_BEGIN(T) FROMJSON_BASE(B) FROMJSON_MEMBER_N(__VA_ARGS__) FROMJSON_END)

using json = nlohmann::json;

/*
Serializing and deserializing possible for glm vector and matrix types
*/
namespace glm {
    void ENGINE_EXPORT to_json(json &js, const glm::vec3 &v);
    void ENGINE_EXPORT to_json(json &js, const glm::vec4 &v);
    void ENGINE_EXPORT to_json(json &js, const glm::mat3 &m);
    void ENGINE_EXPORT to_json(json &js, const glm::mat4 &m);
    void ENGINE_EXPORT from_json(const json &js, glm::vec3 &v);
    void ENGINE_EXPORT from_json(const json &js, glm::vec4 &v);
    void ENGINE_EXPORT from_json(const json &js, glm::mat3 &m);
    void ENGINE_EXPORT from_json(const json &js, glm::mat4 &m);
}

namespace Engine {

    class Object;

    /*
    Type information needed for object serialization and deserialization.
    */
    class ENGINE_EXPORT Type final {
    public:
        typedef void (*InstantiateFunction)(const std::string &);
        typedef void (*SerializeFunction)(json &, const Object *);
        typedef void (*DeserializeFunction)(const json &, Object *);

    private:
        static std::unordered_map<std::string, Type *> typemap;

    public:
        static Type *Find(const std::string &name) {
            auto it = typemap.find(name);
            if (it == typemap.end()) {
                return nullptr;
            }
            return it->second;
        }

    private:
        std::string name;
        InstantiateFunction inst;
        SerializeFunction ser;
        DeserializeFunction deser;

    public:
        Type(const std::string &name, InstantiateFunction inst, SerializeFunction ser, DeserializeFunction deser);
        ~Type();

        std::string const &GetName() { return name; }
        void Instantiate(const std::string &name) { inst(name); }
        void Serialize(json &js, const Object *obj) { ser(js, obj); }
        void Deserialize(const json &js, Object *obj) { deser(js, obj); }
    };
}

struct null {};

template <typename ...> struct concat;

template <template <typename ...> class List, typename T>
struct concat<List<null>, T>
{
    typedef List<T> type;
};

template <template <typename ...> class List, typename ...Types, typename T>
struct concat<List<Types...>, T>
{
    typedef List<Types..., T> type;
};

template <typename...> struct type_list {};

template <>
struct type_list<null> {};
typedef type_list<null> TypeListNull;
#define TYPE_LIST TypeListNull

#endif
