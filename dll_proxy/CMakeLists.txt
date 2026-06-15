cmake_minimum_required(VERSION 3.22)
project(dll_proxy LANGUAGES CXX ASM_MASM)

add_library(${PROJECT_NAME} STATIC dwmapi.cpp)
if(CMAKE_SIZEOF_VOID_P EQUAL 8) # x64
    target_sources(${PROJECT_NAME} PRIVATE dwmapi.asm)
else()                          # x86
    target_sources(${PROJECT_NAME} PRIVATE dwmapi32.asm)
endif()

target_link_options(${PROJECT_NAME} INTERFACE
    "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/dwmapi.def"
    # /SAFESEH:NO # Fix x86 build with Visual Studio 17 2022 Generator.
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        .
)

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    include(../cmake/profile/release.cmake)
else()
    include(../cmake/profile/debug.cmake)
endif()
