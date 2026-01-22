set(KITTYMEMORY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/extern/KittyMemory)

file(GLOB KITTYMEMORY_SRC CONFIGURE_DEPENDS ${KITTYMEMORY_PATH}/KittyMemory/*.cpp)

add_library(KittyMemory STATIC)
target_sources(KittyMemory PRIVATE ${KITTYMEMORY_SRC})
target_include_directories(KittyMemory PUBLIC ${KITTYMEMORY_PATH})
target_compile_definitions(KittyMemory PUBLIC kNO_KEYSTONE)
