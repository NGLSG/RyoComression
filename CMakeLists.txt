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
set(Links minizip lib ssl crypto)
add_definitions(-DBORINGSSL_IMPLEMENTATION)
add_library(RyoCompress SHARED STATIC ${src} ${header})
target_link_libraries(RyoCompress PUBLIC ${Links} archive)
target_include_directories(RyoCompress PUBLIC .)
target_precompile_headers(RyoCompress PRIVATE Header.h)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")
endif ()
add_library(Ryo::Compress ALIAS RyoCompress)