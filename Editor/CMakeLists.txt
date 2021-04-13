project(Editor LANGUAGES CXX)

file(GLOB_RECURSE EDITOR_SOURCE "*.hpp" "*.cpp")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${EDITOR_SOURCE})

add_executable(Editor ${EDITOR_SOURCE})

target_compile_definitions(Editor PRIVATE -DNEWBIE_PATH=u8"${CMAKE_SOURCE_DIR}")
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
