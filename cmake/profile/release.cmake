if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -O2
        -fvisibility=hidden
        -fno-rtti
        -flto
        -fdata-sections
        -ffunction-sections)
    target_link_options(${PROJECT_NAME} PRIVATE
        -flto
        -Wl,--gc-sections)
    # Strip
    if (PROJECT_IS_TOP_LEVEL)
        add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_STRIP} --strip-unneeded "$<TARGET_FILE:${PROJECT_NAME}>")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /MT)
    target_compile_options(
        ${PROJECT_NAME} PRIVATE
        /O2
        /GL
        /GR-
        # /EHs-c-
        /EHsc)
    target_link_options(
        ${PROJECT_NAME}
        PRIVATE /LTCG /OPT:REF /OPT:ICF /RELEASE)
endif()
