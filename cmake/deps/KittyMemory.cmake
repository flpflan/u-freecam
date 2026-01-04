set(KITTYMEMORY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/extern/KittyMemory)

file(GLOB KITTYMEMORY_SRC ${KITTYMEMORY_PATH}/KittyMemory/*.cpp)

add_library(KittyMemory INTERFACE)
target_sources(KittyMemory INTERFACE ${KITTYMEMORY_SRC})
target_include_directories(KittyMemory INTERFACE ${KITTYMEMORY_PATH})
target_compile_definitions(KittyMemory INTERFACE kNO_KEYSTONE)
