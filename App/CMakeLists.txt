cmake_minimum_required(VERSION 3.8)

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE PATH "toolchain file")
endif()

project(App LANGUAGES CXX)

set(Engine_DIR "C:/Newbie/Build/Engine")
find_package(Engine REQUIRED)

file(GLOB_RECURSE APP_SOURCE "*.hpp" "*.cpp")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${APP_SOURCE})

add_executable(App ${APP_SOURCE})

target_link_libraries(App PRIVATE Engine)
target_include_directories(App
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(
  App PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)

add_custom_command(TARGET App POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Engine> $<TARGET_FILE_DIR:App>
)
