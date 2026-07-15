// 文件 main.cpp

#include "math.hpp"

#include <iostream>
#include <type_traits>

// 下面这个函数的实现在文件 math_int.cpp中
// 这里偷懒没有写 单独的函数声明头文件
// 所以，在使用前这里声明
int add_int(int, int);

int main() {
    auto a = add(1, 2);
    auto b = add_int(1, 2);

    return 0;
}