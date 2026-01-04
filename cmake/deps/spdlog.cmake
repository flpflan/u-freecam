set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_BENCH OFF CACHE BOOL "" FORCE)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/spdlog/include)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern/spdlog)
