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
set(Links archive minizip lib ssl crypto)
add_definitions(-DBORINGSSL_IMPLEMENTATION)
add_library(RyoCompress-static STATIC ${src} ${header})
target_link_libraries(RyoCompress-static PUBLIC ${Links})
target_include_directories(RyoCompress-static PUBLIC .)
target_precompile_headers(RyoCompress-static PRIVATE Header.h)

add_library(RyoCompress-shared SHARED ${src} ${header})
target_link_libraries(RyoCompress-shared PUBLIC ${Links})
target_include_directories(RyoCompress-shared PUBLIC .)
target_precompile_headers(RyoCompress-shared PRIVATE Header.h)
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")
endif()
add_library(RyoCompress ALIAS RyoCompress-static)