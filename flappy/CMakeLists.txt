cmake_minimum_required(VERSION 3.10)

project(flappy C)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
add_executable(${PROJECT_NAME} ${SOURCES})

include(${CMAKE_SOURCE_DIR}/_cmake/compiler_options.cmake)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_dependencies(${PROJECT_NAME} copy_assets)
