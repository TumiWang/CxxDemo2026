// 文件 main.cpp

#include "math.hpp"

#include <iostream>
#include <type_traits>

#define PRINT_VARIABLE_INFO(v)  \
    std::cout << "变量" << #v << "是否为浮点数:"    \
        << std::is_floating_point<decltype(v)>::value   \
        << " 内存占用(字节):" <<sizeof(v) << std::endl;

int main() {
    auto a = add(1, 2);
    auto b = add(1.0, 2.0);
    auto c = add(1.0f, 2.0f);

    std::cout << std::boolalpha;

    PRINT_VARIABLE_INFO(a);
    PRINT_VARIABLE_INFO(b);
    PRINT_VARIABLE_INFO(c);

    // auto d = add(1.0, 2.0f);
    // 注意: 上面这句会编译失败
    // 1.0  编译器认为是双精度的浮点数 -- double
    // 2.0f 编译器认为是单精度的浮点数 -- float
    // 这导致add函数的两个参数类型不一致

    return 0;
}