# 文件 CMakeLists.txt

cmake_minimum_required(VERSION 3.16)

project(template)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

if (APPLE)
    set(CMAKE_INSTALL_RPATH "@executable_path")
else()
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
endif ()

add_library(add_math SHARED math.cpp)

add_executable(test main.cpp)

target_link_libraries(test PRIVATE add_math)

install(TARGETS test add_math DESTINATION .)