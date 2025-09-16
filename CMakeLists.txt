cmake_minimum_required(VERSION 3.22)
project(freecam LANGUAGES C CXX)

if(NOT TARGET_PLATFORM)
    set(TARGET_PLATFORM "ANDROID")
endif()
string(TOUPPER ${TARGET_PLATFORM} TARGET_PLATFORM)

if(${TARGET_PLATFORM} STREQUAL "ANDROID")
    set(ANDROID_NDK "/opt/android-ndk")
    set(CMAKE_C_COMPILER "${ANDROID_NDK}/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android24-clang")
    set(CMAKE_CXX_COMPILER "${ANDROID_NDK}/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android24-clang++")
    add_definitions(-DWINDOWS_MODE=0 -DANDROID_MODE=1 -DLINUX_MODE=0)
elseif(${TARGET_PLATFORM} STREQUAL "WINDOWS")
    add_definitions(-DWINDOWS_MODE=1 -DANDROID_MODE=0 -DLINUX_MODE=0)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB BY_OPEN_SRC
    extern/byOpen/src/native/prefix.h
    extern/byOpen/src/native/byopen.h
    extern/byOpen/src/native/byopen_android.c
)

include_directories(include)

add_subdirectory(src)
