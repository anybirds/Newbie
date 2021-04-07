project(HeaderTool LANGUAGES CXX)

file(GLOB_RECURSE HEADERTOOL_HEADER "*.hpp")
file(GLOB_RECURSE HEADERTOOL_SOURCE "*.cpp")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADERTOOL_HEADER} ${HEADERTOOL_SOURCE})

add_executable(HeaderTool ${HEADERTOOL_SOURCE})

target_include_directories(HeaderTool
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)
set_target_properties(
  HeaderTool PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)

install(TARGETS HeaderTool EXPORT HeaderTool DESTINATION lib)
install(FILES ${HEADERTOOL_HEADER} DESTINATION include)

install(EXPORT HeaderTool
  FILE HeaderTool.cmake
  DESTINATION lib/cmake/HeaderTool
)

include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/HeaderToolConfig.cmake"
  INSTALL_DESTINATION "lib/cmake/HeaderTool"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/HeaderToolConfig.cmake
  DESTINATION lib/cmake/HeaderTool
)

export(EXPORT HeaderTool
  FILE "${CMAKE_CURRENT_BINARY_DIR}/HeaderTool.cmake"
)
