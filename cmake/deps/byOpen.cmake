set(BY_OPEN_BASE ${CMAKE_CURRENT_SOURCE_DIR}/extern/byOpen/src/native)
file(GLOB BY_OPEN_SRC
    ${BY_OPEN_BASE}/byopen_android.c
)

add_library(by_open INTERFACE)
target_sources(by_open INTERFACE ${BY_OPEN_SRC})
target_include_directories(by_open INTERFACE ${BY_OPEN_BASE})
