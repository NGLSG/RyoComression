cmake_minimum_required(VERSION 3.28)

# 添加第三方库的子目录
add_subdirectory(libarchive)
add_subdirectory(zlib)
# 创建一个 INTERFACE 库
add_library(lib INTERFACE)
# 为目标 lib 添加包含目录
file(GLOB msrc
        zlib/contrib/minizip/*.c
)
file(GLOB mheader
        zlib/contrib/minizip/*.h
)
add_library(minizip STATIC ${msrc} ${mheader})
target_link_libraries(minizip PUBLIC zlib)
target_include_directories(lib INTERFACE libarchive/libarchive zlib/contrib)