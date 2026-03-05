set(CMAKE_C_COMPILER clang-20)
set(CMAKE_CXX_COMPILER clang++-20)

set(CMAKE_C_FLAGS_INIT "-fuse-ld=lld -Wno-unused-command-line-argument")
set(CMAKE_CXX_FLAGS_INIT "-fuse-ld=lld -Wno-c++11-narrowing  -Wno-unused-command-line-argument")

set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} -I/usr/include/llvm-20")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -I/usr/include/llvm-20")
