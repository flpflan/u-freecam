if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE release CACHE STRING "Build Type" FORCE)
endif()

set(SOURCE_FILES
    lib.cpp
    bootstrap.cpp
    core.cpp
    proxy/camera.cpp
    proxy/cursor.cpp
)
if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    list(APPEND SOURCE_FILES ${BY_OPEN_SRC})
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog_header_only)
target_link_libraries(${PROJECT_NAME} PRIVATE dobby)

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE dl)
    target_compile_options(${PROJECT_NAME} PRIVATE -fPIC)
    target_link_options(${PROJECT_NAME} PRIVATE -fseh-exceptions -static-libstdc++)
elseif(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /MT)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/..
)
