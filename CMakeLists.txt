cmake_minimum_required(VERSION 3.22)
set(name RyoCompress)
set(CMAKE_CXX_STANDARD 17)
project(${name})
if (ANDROID)
    set(SSL_LIB_NAME ssl)
    add_library(${SSL_LIB_NAME} SHARED IMPORTED)
    set_target_properties(${SSL_LIB_NAME} PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/Libs/${ANDROID_ABI}/openssl/lib/lib${SSL_LIB_NAME}.so)

    set(CRYPTO_LIB_NAME crypto)
    add_library(${CRYPTO_LIB_NAME} SHARED IMPORTED)
    set_target_properties(${CRYPTO_LIB_NAME} PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/Libs/${ANDROID_ABI}/openssl/lib/lib${CRYPTO_LIB_NAME}.so)
    set(Links minizip lib archive ssl crypto)
    include_directories(${PROJECT_SOURCE_DIR}/Libs/${ANDROID_ABI}/openssl/include)
else ()
    find_package(OpenSSL REQUIRED)
    set(Links minizip lib archive OpenSSL::SSL OpenSSL::Crypto)
endif ()
file(GLOB src
        *.cpp
)
file(GLOB header
        *.hpp
        *.h
        EXCLUDE Header.h
)
add_subdirectory(Library)

add_library(RyoCompress SHARED STATIC ${src} ${header})
target_link_libraries(RyoCompress PUBLIC ${Links})
target_include_directories(RyoCompress PUBLIC .)
target_precompile_headers(RyoCompress PRIVATE Header.h)
if (ANDROID)
    target_include_directories(RyoCompress PUBLIC ${PROJECT_SOURCE_DIR}/Libs/${ANDROID_ABI}/openssl/include)
endif ()

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")
endif ()
add_library(Ryo::Compress ALIAS RyoCompress)