# 文件 CMakeLists.txt

cmake_minimum_required(VERSION 3.16)

project(test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

# process 从 boost 1.64.0 开始支持
find_package(Boost 1.64.0 REQUIRED COMPONENTS filesystem process CONFIG)

add_executable(test main.cpp)

target_link_libraries(test PRIVATE Boost::filesystem Boost::process)

if(LINUX)
    target_link_libraries(test PRIVATE pthread)
endif()

install(TARGETS test DESTINATION .)

install(FILES $<TARGET_RUNTIME_DLLS:test> DESTINATION .)