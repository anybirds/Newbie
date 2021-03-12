cmake_minimum_required(VERSION 3.8)

if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE PATH "toolchain file")
endif()

project(User LANGUAGES CXX)

set(Engine_DIR "C:/Newbie/Build/Engine")
find_package(Engine REQUIRED)

file(GLOB_RECURSE USER_SOURCE "*.hpp" "*.cpp")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${USER_SOURCE})

add_library(User SHARED ${USER_SOURCE})

target_link_libraries(User PRIVATE Engine)
target_include_directories(User
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(
  User PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)

add_custom_command(TARGET User POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Engine> $<TARGET_FILE_DIR:User>
)
