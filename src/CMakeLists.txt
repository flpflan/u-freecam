if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build Type" FORCE)
endif()

file(GLOB FREECAM_SRC
    *.cpp
    proxy/*.cpp
    feature/*.cpp
)

if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    list(APPEND FREECAM_SRC ${BY_OPEN_SRC})
    list(APPEND FREECAM_SRC ${KITTYMEMORY_SRC})
endif()

add_library(${PROJECT_NAME} SHARED ${FREECAM_SRC})

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
