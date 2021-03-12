cmake_minimum_required(VERSION 3.8)

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE PATH "toolchain file")
endif()

project(Engine LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Assimp REQUIRED)
find_package(stb REQUIRED)
find_package(ZLIB REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

file(GLOB_RECURSE ENGINE_HEADER "*.hpp")
file(GLOB_RECURSE ENGINE_SOURCE "*.cpp")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_HEADER} ${ENGINE_SOURCE})

add_library(Engine SHARED ${ENGINE_HEADER} ${ENGINE_SOURCE})

target_compile_definitions(Engine PRIVATE ENGINE_LIBRARY)
target_link_libraries(Engine PUBLIC glfw GLEW::GLEW ${OPENGL_LIBRARY} glm assimp::assimp nlohmann_json nlohmann_json::nlohmann_json)
target_include_directories(Engine
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)
set_target_properties(
  Engine PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)

install(TARGETS Engine EXPORT Engine DESTINATION lib)
install(FILES ${ENGINE_HEADER} DESTINATION include)

include(InstallRequiredSystemLibraries)
include(CPack)

install(EXPORT Engine
  FILE Engine.cmake
  DESTINATION lib/cmake/Engine
)

include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/EngineConfig.cmake"
  INSTALL_DESTINATION "lib/cmake/example"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/EngineConfig.cmake
  DESTINATION lib/cmake/Engine
)

export(EXPORT Engine
  FILE "${CMAKE_CURRENT_BINARY_DIR}/Engine.cmake"
)
