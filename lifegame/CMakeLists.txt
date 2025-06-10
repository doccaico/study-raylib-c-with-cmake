cmake_minimum_required(VERSION 3.10)

project(lifegame C)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
add_executable(${PROJECT_NAME} ${SOURCES})

if (MSVC)
    set(CMAKE_C_FLAGS "")
    set(CMAKE_C_FLAGS_DEBUG "")
    set(CMAKE_C_FLAGS_RELEASE "")

    target_compile_features(${PROJECT_NAME} PRIVATE c_std_11)

    target_compile_options(${PROJECT_NAME} PRIVATE
        /utf-8 -Oi -fp:precise -MP -FC -GF)
    target_compile_options(${PROJECT_NAME} PRIVATE
        -W4 -WX -wd4100 -wd4101 -wd4127 -wd4146 -wd4505 -wd4456 -wd4457)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        -DUNICODE -D_UNICODE)

    # Release
    target_compile_options(${PROJECT_NAME} PRIVATE 
        $<$<CONFIG:Release>: -O2 -MT>)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:NDEBUG _NDEBUG>)

    # Debug
    target_compile_options(${PROJECT_NAME} PRIVATE 
        $<$<CONFIG:Debug>: -Od -MDd>)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:DEBUG _DEBUG>)

    target_include_directories(${PROJECT_NAME} PRIVATE 
        ${CMAKE_SOURCE_DIR}/vendor/stc/include
        ${CMAKE_SOURCE_DIR}/vendor/raylib/include)
endif()
