# 文件 CMakeLists.txt

cmake_minimum_required(VERSION 3.16)

project(asio-io_context)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

# 需要 boost 的 asio 只有头文件
# asio 从 boost 1.35.0 开始支持
find_package(Boost 1.35.0 REQUIRED)

add_executable(test main.cpp)

target_link_libraries(test PRIVATE Boost::headers)

if(LINUX)
    target_link_libraries(test PRIVATE pthread)
endif()

install(TARGETS test DESTINATION .)