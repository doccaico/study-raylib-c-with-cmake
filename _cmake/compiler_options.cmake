if (MSVC)
    set(CMAKE_C_FLAGS "")
    set(CMAKE_C_FLAGS_DEBUG "")
    set(CMAKE_C_FLAGS_RELEASE "")

    target_compile_features(${PROJECT_NAME} PRIVATE c_std_11)

    target_compile_options(${PROJECT_NAME} PRIVATE
        /utf-8 -Oi -fp:precise -MP -FC -GF)
    target_compile_options(${PROJECT_NAME} PRIVATE
        -W4 -wd4100 -wd4101 -wd4127 -wd4146 -wd4505 -wd4456 -wd4457)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS)
    target_link_options(${PROJECT_NAME} PRIVATE
        /libpath:${CMAKE_SOURCE_DIR}/_vendor/raylib/lib)

    # Debug
    target_compile_options(${PROJECT_NAME} PRIVATE 
        $<$<CONFIG:Debug>: -Od -MDd -Zi>)
    target_compile_options(${PROJECT_NAME} PRIVATE 
        $<$<CONFIG:Debug>: -fsanitize=address -fsanitize=fuzzer>)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:DEBUG _DEBUG>)
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:/NODEFAULTLIB:msvcrt>)

    # Release
    target_compile_options(${PROJECT_NAME} PRIVATE 
        $<$<CONFIG:Release>: -O2 -MT>)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:NDEBUG _NDEBUG>)
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup /NODEFAULTLIB:libcmt>)

    target_include_directories(${PROJECT_NAME} PRIVATE 
        ${CMAKE_SOURCE_DIR}/_vendor/yac
        ${CMAKE_SOURCE_DIR}/_vendor/raylib/include)

    target_link_libraries(${PROJECT_NAME} PRIVATE raylib.lib winmm.lib)
endif()
