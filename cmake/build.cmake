# ===== Config output directory =====

# arch
if(CMAKE_SYSTEM_PROCESSOR)
    string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" ARCH)
else()
    set(ARCH "unknown")
endif()

if(ARCH MATCHES "amd64|x86_64")
    set(ARCH x86_64)
elseif(ARCH MATCHES "aarch64|arm64")
    set(ARCH aarch64)
endif()

# platform
if(ANDROID)
    set(PLATFORM android)
elseif(WIN32)
    set(PLATFORM windows)
elseif(APPLE)
    set(PLATFORM macos)
elseif(UNIX)
    set(PLATFORM linux)
else()
    set(PLATFORM unknown)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    OUTPUT_NAME "${PROJECT_NAME}-${ARCH}-${PLATFORM}"
)
unset(ARCH_SUFFIX)

# ===== Profile =====
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/profile/release.cmake)
else()
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/profile/debug.cmake)
endif()

