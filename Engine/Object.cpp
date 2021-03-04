#include <Common/Debug.hpp>
#include <Common/Object.hpp>
#include <Common/Resource.hpp>

using namespace std;
using namespace Engine;

namespace Engine {
    TYPE_DEF(Object)

    void to_json(json &js, const Object *obj) {
        if (obj == nullptr) {
            js = nullptr;
        } else {
            js = obj->GetName();
        }
    }

    void to_json(json &js, const Object &obj) {

    }

    void from_json(const json &js, Object *&obj) {
        if (js.is_null()) {
            obj = nullptr;
        } else {
            std::string name = js.get<std::string>();
            obj = Engine::Find<Object>(name);
            Resource *res = dynamic_cast<Resource *>(obj);
            if (res) {
                res->shouldLoad = Resource::sceneLoad;
            }
        }
    }

    void from_json(const json &js, Object &obj) {

    }

    void Destroy(Object *obj)  {
        if (IsValid(obj)) {
            Object::des.insert(obj);
        }
    }

    bool IsValid(Object *obj) {
        auto it = Object::objs.find(obj);
        if (it == Object::objs.end()) {
            return false;
        }
        return true;
    }

    Type *GetType(Object *obj) {
        auto it = Object::objs.find(obj);
        if (it == Object::objs.end()) {
            return nullptr;
        }
        return it->second;
    }
}

unordered_map<Object *, Type *> Object::objs;
unordered_map<string, Object *> Object::objmap;
unordered_set<Object *> Object::des;

void Object::Destruct() {
    for (Object *obj : des) {
#ifdef DEBUG
        cout << '[' << __FUNCTION__ << ']' << " destroy " << obj->name << endl;
#endif
        delete obj;
    }
    des.clear();
}

Object::Object(const string &name, Type *type) : name(name) {
    objs.insert({this, type});
    objmap.insert({name, this});
}

Object::~Object() {
    // calling a virtual function in destructor does not invoke the derived class function
    // OnDestroy();

    objs.erase(this);
    objmap.erase(name);
}
