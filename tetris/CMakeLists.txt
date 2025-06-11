cmake_minimum_required(VERSION 3.10)

project(tetris C)

file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/containers/*.c)

add_executable(${PROJECT_NAME} ${SOURCES})

include(${CMAKE_SOURCE_DIR}/_cmake/compiler_options.cmake)

# link_directories("/projectspath/LinkTest/TestLib/app/build/intermediates/cmake/debug/obj/armeabi-v7a")

# set(MY_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/containers/vec.c)
# add_library(vec SHARED ${MY_SOURCES})

# add_library(veclib STATIC ${MY_SOURCES})
# target_link_libraries(veclib PRIVATE )


# test (STC)
set(MY_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/containers/vec.c)
# add_library(vec.lib SHARED ${MY_SOURCES})
add_library(vec STATIC ${MY_SOURCES})
target_compile_features(vec PRIVATE c_std_11)
target_include_directories(vec PRIVATE 
    ${CMAKE_SOURCE_DIR}/_vendor/stc/include
    ${CMAKE_SOURCE_DIR}/_vendor/raylib/include)

target_link_options(${PROJECT_NAME} PRIVATE
    /libpath:${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE vec)
