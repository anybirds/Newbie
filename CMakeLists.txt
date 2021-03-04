cmake_minimum_required(VERSION 3.8)

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "C:\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake" CACHE PATH "toolchain file")
endif()

project(Newbie)

list (APPEND CMAKE_PREFIX_PATH "C:\\vcpkg\\installed\\x64-windows")

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Assimp REQUIRED)
find_package(SOIL CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_subdirectory(Engine)
add_subdirectory(Custom)
add_subdirectory(App)
add_subdirectory(Editor)

add_dependencies(Custom Engine)
add_dependencies(Editor Engine)
add_dependencies(App Engine)
