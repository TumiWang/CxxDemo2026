# 交叉编译
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(TARGET_TRIPLE aarch64-linux-gnueabihf)

# 指定使用Ninja
set(CMAKE_GENERATOR "Ninja" CACHE STRING "")

# 这是我本地环境 aarch64 ubuntu 需要的SDK的位置
set(CMAKE_SYSROOT /sdk/ubuntu20-aarch64)

set(CMAKE_C_FLAGS_INIT "-fuse-ld=lld -Wno-unused-command-line-argument")
set(CMAKE_CXX_FLAGS_INIT "-fuse-ld=lld -Wno-c++11-narrowing  -Wno-unused-command-line-argument")

set(CMAKE_C_COMPILER clang-20)
set(CMAKE_CXX_COMPILER clang++-20)

set(CMAKE_C_COMPILER_TARGET ${TARGET_TRIPLE})
set(CMAKE_CXX_COMPILER_TARGET ${TARGET_TRIPLE})

set(CMAKE_LINKER "ld.lld" CACHE STRING "")

# 下面两个是静态库需要的工具
set(CMAKE_AR "llvm-ar-20" CACHE STRING "")
set(CMAKE_RANLIB "llvm-ranlib-20" CACHE STRING "")

# set(CMAKE_RC_COMPILER "llvm-rc-20" CACHE STRING "")

set(Boost_USE_STATIC_LIBS ON)
set(OPENSSL_USE_STATIC_LIBS ON)

# 下面通常不需要设置
add_compile_definitions(__LINUX__)
set(LINUX ON CACHE BOOL "")