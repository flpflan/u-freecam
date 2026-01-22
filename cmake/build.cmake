# Config output directory
if(ANDROID)
    set(ARCH_SUFFIX ${CMAKE_ANDROID_ARCH_ABI})
else()
    set(ARCH_SUFFIX ${CMAKE_SYSTEM_PROCESSOR})
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    OUTPUT_NAME "${PROJECT_NAME}-${ARCH_SUFFIX}"
)
unset(ARCH_SUFFIX)

# Profile
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/profile/release.cmake)
else()
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/profile/debug.cmake)
endif()

