#ifndef SCRIPT_H
#define SCRIPT_H

#include <unordered_set>
#include <engine_global.hpp>

namespace Engine {
    class ENGINE_EXPORT Script {
    private:
        static std::unordered_set<Script *> scriptset;

    public:
        static void Start();
        static void Update();

    public:
        Script();
        virtual ~Script();

        virtual void OnStart() {}
        virtual void OnUpdate() {}
    };
}

#endif
