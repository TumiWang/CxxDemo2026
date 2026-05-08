# 文件 CMakeLists.txt

cmake_minimum_required(VERSION 3.16)

project(rtti)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

# 使用静态的 boost 
# 我的编译环境 只有静态库
set(Boost_USE_STATIC_LIBS ON)

# coroutine2 从 boost 1.59.0 开始支持
# 但 coroutine2 新的 boost 内部需要 fiber
# fiber 从 boost 1.62.0 开始支持
find_package(Boost 1.62.0 REQUIRED fiber)

add_executable(test main.cpp)

target_link_libraries(test PRIVATE Boost::fiber)

install(TARGETS test DESTINATION .)