cmake_minimum_required(VERSION 3.15)

project(tests LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)
enable_testing()

option(TEST_ENABLE_SANITIZER ON)

file(GLOB_RECURSE TEST_SRCS ${CMAKE_SOURCE_DIR}/*.cc)

include_directories(${CMAKE_SOURCE_DIR}/../include)

if (MSVC)
    add_compile_options(/nologo /Zc:preprocessor /utf-8 /DNOMINMAX /bigobj /GR-)
else()
    add_compile_options(-Wall -Wextra -Werror -nostdlib -fno-exceptions -fno-rtti -fno-unwind-tables -fno-asynchronous-unwind-tables -Wno-unused-command-line-argument)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND TEST_ENABLE_SANITIZER EQUAL ON)
        add_compile_options(-fuse-ld=lld -fsanitize=address,undefined)
        add_link_options(-fuse-ld=lld -fsanitize=address,undefined)
    endif()
    if (WIN32)
        add_compile_options(-finput-charset=utf-8 -fexec-charset=utf-8)
    endif()
endif()

foreach (a_test IN LISTS TEST_SRCS)
    get_filename_component(filename ${a_test} NAME_WE)
    add_executable(${filename} ${a_test})
    add_test(NAME ${filename} COMMAND ${CMAKE_BINARY_DIR}/${filename})
endforeach()
