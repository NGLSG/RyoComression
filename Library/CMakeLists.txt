cmake_minimum_required(VERSION 3.28)

add_subdirectory(libarchive)
add_subdirectory(zlib)
add_subdirectory(boringssl)

add_library(lib INTERFACE)

file(GLOB msrc
        zlib/contrib/minizip/*.c
)
file(GLOB mheader
        zlib/contrib/minizip/*.h
)
add_library(minizip STATIC ${msrc} ${mheader})
target_link_libraries(minizip PUBLIC zlib)
target_include_directories(lib INTERFACE libarchive/libarchive zlib/contrib)