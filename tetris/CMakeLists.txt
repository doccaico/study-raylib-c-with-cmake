cmake_minimum_required(VERSION 3.10)

project(tetris C)

file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_executable(${PROJECT_NAME} ${SOURCES})

include(${CMAKE_SOURCE_DIR}/_cmake/compiler_options.cmake)
