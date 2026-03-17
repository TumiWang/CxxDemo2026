# 文件 CMakeLists.txt

cmake_minimum_required(VERSION 3.16)

project(baidu)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

# 需要 boost 的 asio 和 beast 它们都只有头文件
# beast 从 boost 1.66.0 开始支持
# asio 从 boost 1.35.0 开始支持
find_package(Boost 1.66.0 REQUIRED)

set(OPENSSL_USE_STATIC_LIBS ON)

# 因为使用 https 所以需要 OpenSSL
find_package(OpenSSL REQUIRED)

add_executable(test main.cpp)

target_link_libraries(test PRIVATE Boost::headers OpenSSL::SSL)

if(LINUX)
    target_link_libraries(test PRIVATE pthread)
endif()

install(TARGETS test DESTINATION .)