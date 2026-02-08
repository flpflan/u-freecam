cmake_minimum_required(VERSION 3.22)
project(webui LANGUAGES CXX)

set(HTTPLIB_USE_OPENSSL_IF_AVAILABLE OFF CACHE BOOL "Use openssl")
set(HTTPLIB_USE_ZLIB_IF_AVAILABLE OFF CACHE BOOL "Use zlib")
set(HTTPLIB_USE_BROTLI_IF_AVAILABLE OFF CACHE BOOL "Use brotli")
set(HTTPLIB_USE_ZSTD_IF_AVAILABLE OFF CACHE BOOL "Use zstd")
add_subdirectory(../extern/cpp-httplib cpp-httplib)

file(GLOB_RECURSE WEBUI_SRC CONFIGURE_DEPENDS ./*.cpp)

add_library(${PROJECT_NAME} STATIC ${WEBUI_SRC})

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        .
        ../include
        ../extern/KittyMemory
        ../extern/xDL/xdl/src/main/cpp/include
)

target_link_libraries(webui PRIVATE httplib::httplib)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /Zc:preprocessor)
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    include(../cmake/profile/release.cmake)
else()
    include(../cmake/profile/debug.cmake)
endif()
