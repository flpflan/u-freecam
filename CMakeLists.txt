cmake_minimum_required(VERSION 3.22)
project(freecam LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "" FORCE)

# Configure CCache if available
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif()

# ===== Options =====
# option(UMOD_ENABLE_WEBUI "Include webui" ON)

# ===== Deps =====
include(cmake/deps/spdlog.cmake)
include(cmake/deps/Dobby.cmake)
include(cmake/deps/UnityResolve.cmake)
if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    include(cmake/deps/xDL.cmake)
    include(cmake/deps/KittyMemory.cmake)
endif()
if(NOT (${CMAKE_SYSTEM_NAME} STREQUAL "Android" AND ${CMAKE_SYSTEM_VERSION} LESS 33))
    include(cmake/deps/backward-cpp.cmake)
endif()

# if(${UMOD_ENABLE_WEBUI})
#     add_subdirectory(webui)
# endif()

# ===== Taget Freecam Definition =====
file(GLOB_RECURSE FREECAM_SRC CONFIGURE_DEPENDS
    src/*.cpp
    src/proxy/*.cpp
    src/free_camera/*.cpp
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    # list(APPEND FREECAM_SRC include/umod/platform/android.cpp)
endif()

add_library(${PROJECT_NAME} SHARED ${FREECAM_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog_header_only dobby)
if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    target_link_libraries(${PROJECT_NAME} PRIVATE xdl KittyMemory)
endif()
if(NOT (${CMAKE_SYSTEM_NAME} STREQUAL "Android" AND ${CMAKE_SYSTEM_VERSION} LESS 33))
    target_link_libraries(${PROJECT_NAME} PRIVATE Backward::Interface)
endif()

# if(${UMOD_ENABLE_WEBUI})
#     target_link_libraries(${PROJECT_NAME} PRIVATE webui)
# endif()

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE dl)
    target_compile_options(${PROJECT_NAME} PRIVATE -fPIC -fdeclspec)
    target_link_options(${PROJECT_NAME} PRIVATE
        -fseh-exceptions
        -Wl,--icf=all
        -Wl,-z,max-page-size=16384)
elseif(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /MT /Zc:preprocessor)
endif()

# ===== Build Config =====
include(cmake/build.cmake)
