# 文件 CMakeLists.txt

cmake_minimum_required(VERSION 3.16)

project(template)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

add_executable(test main.cpp math_int.cpp)

install(TARGETS test DESTINATION .)