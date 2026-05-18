# 交叉编译
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR i686)
set(TARGET_TRIPLE i686-pc-win32)

# 指定使用Ninja
set(CMAKE_GENERATOR "Ninja" CACHE STRING "")

# 这是我本地环境 windows 需要的SDK的位置
set(CMAKE_SYSROOT /sdk/winsdk)

set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} -fuse-ld=lld")
set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} -Wno-unused-command-line-argument")
set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} -L\"${CMAKE_SYSROOT}/windows kits/10/lib/10.0.26100.0/um/x86\"")
set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} -L\"${CMAKE_SYSROOT}/windows kits/10/lib/10.0.26100.0/ucrt/x86\"")
set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} -L\"${CMAKE_SYSROOT}/vc/tools/msvc/14.44.35207/lib/onecore/x86\"")
set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} -I\"${CMAKE_SYSROOT}/vc/tools/msvc/14.44.35207/include\"")
set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} -I\"${CMAKE_SYSROOT}/windows kits/10/include/10.0.26100.0/shared\"")
set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} -I\"${CMAKE_SYSROOT}/windows kits/10/include/10.0.26100.0/um\"")
set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} -I\"${CMAKE_SYSROOT}/windows kits/10/include/10.0.26100.0/ucrt\"")
set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} -I\"${CMAKE_SYSROOT}/windows kits/10/bin/x86/include\"")

set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -fuse-ld=lld")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -Wno-unused-command-line-argument")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -Wno-c++11-narrowing")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -Wno-ignored-attributes -Wno-pragma-pack -Wno-ignored-pragma-intrinsic -Wno-nonportable-include-path")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -L\"${CMAKE_SYSROOT}/windows kits/10/lib/10.0.26100.0/um/x86\"")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -L\"${CMAKE_SYSROOT}/windows kits/10/lib/10.0.26100.0/ucrt/x86\"")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -L\"${CMAKE_SYSROOT}/vc/tools/msvc/14.44.35207/lib/x86\"")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -I\"${CMAKE_SYSROOT}/vc/tools/msvc/14.44.35207/include\"")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -I\"${CMAKE_SYSROOT}/windows kits/10/include/10.0.26100.0/shared\"")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -I\"${CMAKE_SYSROOT}/windows kits/10/include/10.0.26100.0/um\"")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -I\"${CMAKE_SYSROOT}/windows kits/10/include/10.0.26100.0/ucrt\"")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -I\"${CMAKE_SYSROOT}/windows kits/10/bin/x86/include\"")

set(CMAKE_RC_FLAGS_INIT "${CMAKE_RC_FLAGS_INIT} -I\"${CMAKE_SYSROOT}/windows kits/10/include/10.0.26100.0/shared\"")
set(CMAKE_RC_FLAGS_INIT "${CMAKE_RC_FLAGS_INIT} -I\"${CMAKE_SYSROOT}/windows kits/10/include/10.0.26100.0/um\"")

set(CMAKE_C_COMPILER clang-20)
set(CMAKE_CXX_COMPILER clang++-20)

set(CMAKE_C_COMPILER_TARGET ${TARGET_TRIPLE})
set(CMAKE_CXX_COMPILER_TARGET ${TARGET_TRIPLE})

# 静态库需要的工具
set(CMAKE_AR "llvm-ar-20" CACHE STRING "")
set(CMAKE_RANLIB "llvm-ranlib-20" CACHE STRING "")

# 资源编译工具
set(CMAKE_RC_COMPILER "llvm-rc-20" CACHE STRING "")

list(APPEND CMAKE_FIND_ROOT_PATH "${CMAKE_SYSROOT}/boost/win32/lib/cmake")

# 下面通常不需要设置
add_compile_definitions(__WIN32__)
set(WIN32 ON CACHE BOOL ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")