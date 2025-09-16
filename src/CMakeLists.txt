if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE release CACHE STRING "Build Type" FORCE)
endif()

set(SOURCE_FILES
    lib.cpp
    bootstrap.cpp
    freecam.cpp
    camera_proxy.cpp
    cursor_proxy.cpp
)
if(TARGET_PLATFORM STREQUAL "ANDROID")
    list(APPEND SOURCE_FILES ${BY_OPEN_SRC})
endif()

set(EXTERN_BASE ${CMAKE_CURRENT_SOURCE_DIR}/../extern)

set(ANDROID_ABI "arm64-v8a")
include_directories(${EXTERN_BASE}/byOpen/src/native)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE dl)
    target_compile_options(${PROJECT_NAME} PRIVATE -fPIC)
    target_link_options(${PROJECT_NAME} PRIVATE -static-libstdc++ -static-libgcc)
elseif(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /MT)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/..
)
