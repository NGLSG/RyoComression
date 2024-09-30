cmake_minimum_required(VERSION 3.22)
set(name RyoCompress)
set(CMAKE_CXX_STANDARD 17)
project(${name})
find_package(OpenSSL REQUIRED)
file(GLOB src
        *.cpp
)
file(GLOB header
        *.hpp
        *.h
        EXCLUDE Header.h
)
add_subdirectory(Library)
set(Links minizip lib archive OpenSSL::SSL OpenSSL::Crypto)
add_library(RyoCompress SHARED STATIC ${src} ${header})
target_link_libraries(RyoCompress PUBLIC ${Links})
target_include_directories(RyoCompress PUBLIC .)
target_precompile_headers(RyoCompress PRIVATE Header.h)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")
endif ()
add_library(Ryo::Compress ALIAS RyoCompress)