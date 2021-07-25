cmake_minimum_required(VERSION 3.8)

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "${NEWBIE_PATH}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE PATH "toolchain file")
endif()

project(User LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Assimp REQUIRED)
find_package(Stb REQUIRED)
find_package(ZLIB REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(HeaderTool_DIR "${NEWBIE_PATH}/build/HeaderTool")
find_package(HeaderTool REQUIRED)
set(Engine_DIR "${NEWBIE_PATH}/build/Engine")
find_package(Engine REQUIRED)

file(GLOB_RECURSE USER_HEADER "*.hpp")
list(FILTER USER_HEADER EXCLUDE REGEX "${CMAKE_CURRENT_SOURCE_DIR}/build/.*" )
file(GLOB_RECURSE USER_SOURCE "*.cpp")
list(FILTER USER_SOURCE EXCLUDE REGEX "${CMAKE_CURRENT_SOURCE_DIR}/build/.*" )

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${USER_HEADER} ${USER_SOURCE})

add_custom_command(
  OUTPUT generated.cpp 
  COMMAND HeaderTool "${CMAKE_CURRENT_SOURCE_DIR}" > generated.cpp
  DEPENDS HeaderTool ${USER_HEADER}
)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_library(User SHARED ${USER_HEADER} ${USER_SOURCE} generated.cpp)

target_compile_definitions(User PRIVATE USER_LIBRARY)
target_link_libraries(User PRIVATE Engine)
target_include_directories(User
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(
  User PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)