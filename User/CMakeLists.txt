project(User LANGUAGES CXX)

file(GLOB_RECURSE USER_SOURCE "*.hpp" "*.cpp")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${USER_SOURCE})

add_library(USER SHARED ${USER_SOURCE})

target_link_libraries(USER PUBLIC Engine)

target_include_directories(USER
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(USER PRIVATE USER_LIBRARY)

set_target_properties(
  USER PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)
