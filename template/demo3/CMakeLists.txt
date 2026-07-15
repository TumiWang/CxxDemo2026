# 文件 CMakeLists.txt

cmake_minimum_required(VERSION 3.16)

project(template)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

add_library(add_math STATIC math.cpp)

add_executable(test main.cpp)

target_link_libraries(test PRIVATE add_math)

install(TARGETS test DESTINATION .)