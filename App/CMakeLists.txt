project(App LANGUAGES CXX)

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