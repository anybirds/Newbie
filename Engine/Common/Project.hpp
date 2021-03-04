#ifndef PROJECT_H
#define PROJECT_H

#include <string>
#include <unordered_set>

#include <engine_global.hpp>

namespace Engine {

    class Scene;
    class Resource;

    /*
    Static class that manages project loading and saving.

    TODO: Project should manage custom module(.dll) loading and unloading,
    which would involve clearing all Types from the custom module.
    */
    class ENGINE_EXPORT Project final {
    private:
        static std::string path;
        static std::unordered_set<Scene *> sceneset;
        static std::unordered_set<Resource *> resset;

    public:
        static void Clear();
        static bool Load(const std::string &path);
        static void Save();

        friend class Scene;
        friend class Resource;
        friend class ProjectWidget;
    };
}
#endif
