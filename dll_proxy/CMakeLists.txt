cmake_minimum_required(VERSION 3.22)
project(dll_proxy LANGUAGES CXX ASM_MASM)

add_library(${PROJECT_NAME} STATIC dwmapi.asm dwmapi.cpp)

target_link_options(${PROJECT_NAME} INTERFACE
    "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/dwmapi.def"
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