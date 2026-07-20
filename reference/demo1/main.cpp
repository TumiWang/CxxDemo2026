#include <iostream>
#include <type_traits>

#define PRINT_VARIABLE_INFO(v)  \
    if (std::is_lvalue_reference<decltype(v)>::value)   \
        std::cout << "变量 " << #v << " 是左值引用" << std::endl; \
    else if (std::is_rvalue_reference<decltype(v)>::value)   \
        std::cout << "变量 " << #v << " 是右值引用" << std::endl; \
    else std::cout << "变量 " << #v << " 不是引用" << std::endl;

using left_ref = int&;
using right_ref = int&&;

int main() {
    int v0 = 8;

    // v1 v2 v3 都是左值引用
    left_ref v1 = v0;
    left_ref& v2 = v0;
    left_ref&& v3 = v0;

    // v4 也是左值引用
    right_ref& v4 = v0;
    
    // v5 v6 是右值引用
    // v0 是左值，所以 v5和v6 都不能使用v0初始化
    right_ref v5 = 0;
    right_ref&& v6 = 0;

    PRINT_VARIABLE_INFO(v0);
    PRINT_VARIABLE_INFO(v1);
    PRINT_VARIABLE_INFO(v2);
    PRINT_VARIABLE_INFO(v3);
    PRINT_VARIABLE_INFO(v4);
    PRINT_VARIABLE_INFO(v5);
    PRINT_VARIABLE_INFO(v6);

    return 0;
}