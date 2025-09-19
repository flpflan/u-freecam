cmake_minimum_required(VERSION 3.22)
project(freecam LANGUAGES C CXX)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_ANDROID_NDK}/build/cmake/android.toolchain.cmake)
    add_definitions(-DWINDOWS_MODE=0 -DANDROID_MODE=1 -DLINUX_MODE=0)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    add_definitions(-DWINDOWS_MODE=1 -DANDROID_MODE=0 -DLINUX_MODE=0)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "" FORCE)

file(GLOB BY_OPEN_SRC
    extern/byOpen/src/native/prefix.h
    extern/byOpen/src/native/byopen.h
    extern/byOpen/src/native/byopen_android.c
)
include_directories(extern/byOpen/src/native)

set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_BENCH OFF CACHE BOOL "" FORCE)
include_directories(extern/spdlog/include)
add_subdirectory(extern/spdlog)

include_directories(include)
add_subdirectory(src)
