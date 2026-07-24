# 文件 CMakeLists.txt

cmake_minimum_required(VERSION 3.16)

project(lvalue)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/utf-8 /Zc:__cplusplus)
endif()

add_executable(test main.cpp)

install(TARGETS test DESTINATION .)