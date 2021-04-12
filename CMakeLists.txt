cmake_minimum_required(VERSION 3.8)

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/scripts/buildsystems/vcpkg.cmake" CACHE PATH "toolchain file")
endif()

project(Newbie LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Assimp REQUIRED)
find_package(Stb REQUIRED)
find_package(ZLIB REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_subdirectory(HeaderTool)
add_subdirectory(Engine)
add_subdirectory(Editor)
# add_subdirectory(App)

add_dependencies(Engine HeaderTool)
add_dependencies(Editor Engine)
# add_dependencies(App Engine)