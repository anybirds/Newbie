project(HeaderTool LANGUAGES CXX)

file(GLOB_RECURSE HEADERTOOL_SOURCE "*.hpp" "*.cpp")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADERTOOL_SOURCE})

add_executable(HeaderTool ${HEADERTOOL_SOURCE})

target_include_directories(HeaderTool
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(
  HeaderTool PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)