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

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    if(UNIX AND NOT APPLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
        # rtti / lto
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")           
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -O3 -flto")
        # compile section
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdata-sections -ffunction-sections")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdata-sections -ffunction-sections")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections")
        # Optimization level
        # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Oz")
        # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Oz")
        # Strip
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_STRIP} --strip-unneeded "$<TARGET_FILE:${PROJECT_NAME}>")
    elseif(WIN32)
        # set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /O2 /Os /GL /GR- /EHs-c-")
        # set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Os /GL /GR- /EHs-c-")
        # set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /LTCG /OPT:REF /OPT:ICF /RELEASE")
    endif()
endif()

