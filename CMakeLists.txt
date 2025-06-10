cmake_minimum_required(VERSION 3.10)

project(study-raylib-c-with-cmake C)

set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_EXTENSIONS OFF)

add_subdirectory(${CMAKE_SOURCE_DIR}/lifegame)
add_subdirectory(${CMAKE_SOURCE_DIR}/flappy)
