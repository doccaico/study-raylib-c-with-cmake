if (MSVC)
    set(CMAKE_C_FLAGS "")
    set(CMAKE_C_FLAGS_DEBUG "")
    set(CMAKE_C_FLAGS_RELEASE "")

    target_compile_features(yacc PRIVATE c_std_11)

    target_compile_options(yacc PRIVATE
        /utf-8 -Oi -fp:precise -MP -FC -GF)
    target_compile_options(yacc PRIVATE
        -W4 -wd4100 -wd4101 -wd4127 -wd4146 -wd4505 -wd4456 -wd4457)

    # Release
    target_compile_options(yacc PRIVATE 
        $<$<CONFIG:Release>: -O2 -MT>)
    target_compile_definitions(yacc PRIVATE
        $<$<CONFIG:Release>:NDEBUG _NDEBUG>)
    # target_link_options(yacc PRIVATE
    #     $<$<CONFIG:Release>:/ENTRY:mainCRTStartup /NODEFAULTLIB:libcmt>)
    # target_link_options(yacc PRIVATE
    #     $<$<CONFIG:Release>:/ENTRY:mainCRTStartup /NODEFAULTLIB:libcmt>)

    # Debug
    target_compile_options(yacc PRIVATE 
        $<$<CONFIG:Debug>: -Od -MDd>)
    target_compile_definitions(yacc PRIVATE
        $<$<CONFIG:Debug>:DEBUG _DEBUG>)
    # target_link_options(yacc PRIVATE
    #     $<$<CONFIG:Debug>:/NODEFAULTLIB:msvcrt>)

    # target_include_directories(yacc PRIVATE 
    #     ${CMAKE_SOURCE_DIR}/_vendor/yacc
    #     ${CMAKE_SOURCE_DIR}/_vendor/raylib/include)


    # target_link_libraries(yacc PRIVATE raylib.lib winmm.lib)
endif()
