cmake_minimum_required(VERSION 3.28)
set(name RyoCompress)
project(${name})

file(GLOB src
        *.cpp
)
file(GLOB header
        *.hpp
        *.h
        EXCLUDE Header.h
)

add_subdirectory(Library)
add_library(RyoCompress-static STATIC ${src} ${header})
message("Library" ${LibArchive_LIBRARIES})
target_link_libraries(RyoCompress-static PUBLIC archive minizip lib)
target_include_directories(RyoCompress-static PUBLIC .)
target_precompile_headers(RyoCompress-static PRIVATE Header.h)

add_library(RyoCompress ALIAS RyoCompress-static)