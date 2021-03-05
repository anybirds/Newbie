#pragma once

#include <type_traits>

#include <EngineExport.hpp>

namespace Engine {
    
    class Group;

    /*
    Base class for all serializable types
    */
    class ENGINE_EXPORT Entity {
    public:
        Entity() {}
        virtual ~Entity() {}

        virtual void Init() = 0; 
    };

    /*
    if T is the type derived from Entity, puts it to a specified group.
    */
    template <class T, std::enable_if_t<std::is_base_of_v<Entity, T>, bool> = true>
    T *Instantiate(Group *group) {
        group->Insert(new T());
    }
}