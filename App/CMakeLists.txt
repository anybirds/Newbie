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
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
)

add_custom_command(TARGET App POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:GLEW::GLEW> $<TARGET_FILE_DIR:App>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:glfw> $<TARGET_FILE_DIR:App>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:assimp::assimp> $<TARGET_FILE_DIR:App>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:ZLIB::ZLIB> $<TARGET_FILE_DIR:App>

    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Engine> $<TARGET_FILE_DIR:App>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Custom> $<TARGET_FILE_DIR:App>
)
