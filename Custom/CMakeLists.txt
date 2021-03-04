project(Custom LANGUAGES CXX)

file(GLOB_RECURSE CUSTOM_SOURCE "*.hpp" "*.cpp")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CUSTOM_SOURCE})

add_library(Custom SHARED ${CUSTOM_SOURCE})

target_link_libraries(Custom PUBLIC Engine)

target_include_directories(Custom
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(Custom PRIVATE CUSTOM_LIBRARY)

set_target_properties(
  Custom PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
)
