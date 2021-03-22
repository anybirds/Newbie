# Run cmake -G "Visual Studio 15 2017" -A x64 -B ../build/Editor
# Run cmake --build ../build/Editor --config Release

cmake_minimum_required(VERSION 3.8)

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE PATH "toolchain file")
endif()

project(Editor LANGUAGES CXX)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Editor)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Assimp REQUIRED)
find_package(Stb REQUIRED)
find_package(ZLIB REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(Engine_DIR "C:/Newbie/Build/Engine")
find_package(Engine REQUIRED)

file(GLOB_RECURSE EDITOR_SOURCE "*.hpp" "*.cpp")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${EDITOR_SOURCE})

add_executable(Editor ${EDITOR_SOURCE})

target_link_libraries(Editor PRIVATE Engine glfw GLEW::GLEW ${OPENGL_LIBRARY} assimp::assimp nlohmann_json nlohmann_json::nlohmann_json)
target_include_directories(Editor
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(
  Editor PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)

add_custom_command(TARGET Editor PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Engine> $<TARGET_FILE_DIR:Editor>
)
