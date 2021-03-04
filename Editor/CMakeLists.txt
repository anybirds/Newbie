project(Editor LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

file(GLOB_RECURSE EDITOR_SOURCE "*.hpp" "*.h" "*.cpp" "*.ui")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${EDITOR_SOURCE})

add_executable(Editor ${EDITOR_SOURCE})

target_link_libraries(Editor PRIVATE Qt5::Widgets Engine)

target_include_directories(Editor
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(
  Editor PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
)

add_custom_command(TARGET Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:GLEW::GLEW> $<TARGET_FILE_DIR:Editor>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:glfw> $<TARGET_FILE_DIR:Editor>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:assimp::assimp> $<TARGET_FILE_DIR:Editor>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:ZLIB::ZLIB> $<TARGET_FILE_DIR:Editor>

    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Engine> $<TARGET_FILE_DIR:Editor>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Custom> $<TARGET_FILE_DIR:Editor>
)

target_link_libraries(Editor PRIVATE Qt5::Widgets)
