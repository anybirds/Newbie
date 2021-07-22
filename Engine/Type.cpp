#include <Entity.hpp>
#include <Type.hpp>

using json = nlohmann::json;
using namespace std;

Type::Type(const string &name) : name(name) {
    GetAllTypes().insert({name, this});
}

Type::~Type() {
    GetAllTypes().erase(name);
}