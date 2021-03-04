#include <Engine.hpp>

// Add all header files here
#include <Scripts/RotateScript.hpp>

#include <custom_global.hpp>

using namespace Engine;

extern "C" {
    void CUSTOM_EXPORT TypeInit();
}

template <template <typename ...> class List, typename T, typename ...Types>
void type_init();

template <typename T, typename ...Types>
inline void type_init(type_list<T, Types...>) {
    T::type = new Type(T::typeName, T::Instantiate, T::Serialize, T::Deserialize);
    type_init(type_list<Types...>());
}

template <typename T>
inline void type_init(type_list<T>) {
    T::type = new Type(T::typeName, T::Instantiate, T::Serialize, T::Deserialize);
}

void TypeInit() {
    type_init(TYPE_LIST());
}

