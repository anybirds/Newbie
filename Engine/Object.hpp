#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <unordered_map>
#include <unordered_set>

#include <Common/Type.hpp>
#include <engine_global.hpp>

namespace Engine {

    class Project;

    SER_DECL(ENGINE_EXPORT, Object)

    void ENGINE_EXPORT to_json(json &js, const Object *obj);
    void ENGINE_EXPORT from_json(const json &js, Object *&obj);

    void ENGINE_EXPORT Destroy(Object *obj);
    bool ENGINE_EXPORT IsValid(Object *obj);
    Type ENGINE_EXPORT *GetType(Object *obj);

    /*
    Base class for all serializeable objects.
    */
    class ENGINE_EXPORT Object {
        TYPE_DECL(Object)

    private:
        static std::unordered_map<Object *, Type *> objs;
        static std::unordered_map<std::string, Object *> objmap;
        static std::unordered_set<Object *> des;

    public:
        static void Destruct();

    private:
        std::string name;

    public:
        Object(const std::string &name, Type *type = Object::type);
        virtual ~Object();

        std::string const &GetName() const { return name; }
        bool SetName(std::string const &name) {
            auto it = objmap.find(name);
            if (it != objmap.end()) {
                return false;
            }
            objmap.erase(GetName());
            objmap.insert({name, this});
            this->name = name;
            return true;
        }

        // Load and Unload should be considered as pure virtual functions
        virtual void OnInit() { }
        virtual void OnDestroy() { }

        friend void Destroy(Object *);
        friend bool IsValid(Object *);
        friend Type *GetType(Object *);
        template <typename T> friend T *Find(const std::string &);
        friend class Project;
    };

    template <typename T>
    T *Find(const std::string &name) {
        auto it = Object::objmap.find(name);
        if (it == Object::objmap.end()) {
            return nullptr;
        }
        return dynamic_cast<T *>(it->second);
    }
}

typedef typename concat<TYPE_LIST, Engine::Object>::type TypeListObject;
#undef TYPE_LIST
#define TYPE_LIST TypeListObject

#endif
