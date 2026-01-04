cmake_minimum_required(VERSION 3.22)
project(freecam LANGUAGES C CXX)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_ANDROID_NDK}/build/cmake/android.toolchain.cmake)
    set(ANDROID_ABI ${CMAKE_ANDROID_ARCH_ABI})
    add_definitions(-DWINDOWS_MODE=0 -DANDROID_MODE=1 -DLINUX_MODE=0)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    add_definitions(-DWINDOWS_MODE=1 -DANDROID_MODE=0 -DLINUX_MODE=0)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "" FORCE)

# ===== Deps =====
include(cmake/deps/xDL.cmake)
include(cmake/deps/byOpen.cmake)
include(cmake/deps/spdlog.cmake)
include(cmake/deps/Dobby.cmake)
include(cmake/deps/KittyMemory.cmake)
if(NOT (${CMAKE_SYSTEM_NAME} STREQUAL "Android" AND ${CMAKE_SYSTEM_VERSION} LESS 33))
    include(cmake/deps/backward-cpp.cmake)
endif()

# ===== Taget Freecam Definition =====
file(GLOB FREECAM_SRC
    src/*.cpp
    src/proxy/*.cpp
    src/free_camera/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${FREECAM_SRC})

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog_header_only dobby)
if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    target_link_libraries(${PROJECT_NAME} PRIVATE xdl by_open KittyMemory)
endif()
if(NOT (${CMAKE_SYSTEM_NAME} STREQUAL "Android" AND ${CMAKE_SYSTEM_VERSION} LESS 33))
    target_link_libraries(${PROJECT_NAME} PRIVATE Backward::Interface)
endif()

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE dl)
    target_compile_options(${PROJECT_NAME} PRIVATE -fPIC -fdeclspec)
    target_link_options(${PROJECT_NAME} PRIVATE
        -fseh-exceptions
        -Wl,-z,max-page-size=16384)
elseif(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /MT /Zc:preprocessor)
endif()

# ===== Build Config =====
include(cmake/build.cmake)
