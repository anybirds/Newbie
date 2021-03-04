#include <Common/Script.hpp>

using namespace std;
using namespace Engine;

unordered_set<Script *> Script::scriptset;

void Script::Start() {
    for (Script *script : scriptset) {
        script->OnStart();
    }
}

void Script::Update() {
    for (Script *script : scriptset) {
        script->OnUpdate();
    }
}

Script::Script() {
    scriptset.insert(this);
}

Script::~Script() {
    scriptset.erase(this);
}
