set(DOBBY_BASE ${CMAKE_CURRENT_SOURCE_DIR}/extern/Dobby)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(DOBBY_DEBUG ON CACHE BOOL "Build Dobby in Debug mode")
else()
    set(DOBBY_DEBUG OFF CACHE BOOL "Build Dobby in Release mode")
endif()
set(DOBBY_GENERATE_SHARED OFF CACHE BOOL "Build Dobby static")
add_subdirectory(${DOBBY_BASE})
