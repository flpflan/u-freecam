set(XDL_BASE ${CMAKE_CURRENT_SOURCE_DIR}/extern/xDL/xdl/src/main/cpp)

file(GLOB XDL_SRC ${XDL_BASE}/*.c)
add_library(xdl INTERFACE)
target_sources(xdl INTERFACE ${XDL_SRC})
# target_compile_features(xdl PRIVATE c_std_17)
# target_compile_options(xdl PRIVATE -std=c17 -Weverything -Werror)
target_include_directories(xdl INTERFACE ${XDL_BASE}/include ${XDL_BASE})
#
# target_compile_options(xdl PRIVATE -Os -ffunction-sections -fdata-sections)
# target_link_options(xdl PRIVATE -Os -Wl,--exclude-libs,ALL -Wl,--gc-sections)

# if((${ANDROID_ABI} STREQUAL "arm64-v8a") OR (${ANDROID_ABI} STREQUAL "x86_64"))
#     target_link_options(xdl PRIVATE "-Wl,-z,max-page-size=16384")
# endif()
