#include <chrono>
#include <iostream>
#include <string>
#include <type_traits>

#define PRINT_VARIABLE_INFO(v)  \
    if (std::is_lvalue_reference<decltype((v))>::value)   \
        std::cout << #v << " is a lvalue" << std::endl; \
    else if (std::is_rvalue_reference<decltype((v))>::value)   \
        std::cout << #v << " is a xvalue" << std::endl; \
    else std::cout << #v << " is a prvalue" << std::endl;

int main() {
    // 除了字符串以外的字面常量都是 prvalue
    PRINT_VARIABLE_INFO(8); // prvalue
    PRINT_VARIABLE_INFO(true); // prvalue
    PRINT_VARIABLE_INFO(nullptr); // prvalue
    PRINT_VARIABLE_INFO('C'); // prvalue
    PRINT_VARIABLE_INFO("C"); // lvalue
    
#if __cplusplus >= 201402L
    {
        // C++14或更高
        using namespace std::chrono_literals;
        PRINT_VARIABLE_INFO(15s); // prvalue
    }
#endif

    int v0 = 8;
    PRINT_VARIABLE_INFO(v0); // lvalue

    PRINT_VARIABLE_INFO(&v0); // prvalue
    PRINT_VARIABLE_INFO(v0++); // prvalue
    PRINT_VARIABLE_INFO(++v0); // lvalue

    int& v1 = v0;
    int&& v2 = std::move(v0);
    PRINT_VARIABLE_INFO(v1); // lvalue
    PRINT_VARIABLE_INFO(v2); // lvalue
    PRINT_VARIABLE_INFO(std::move(v0)); // xvalue

    PRINT_VARIABLE_INFO(&main); // prvalue
    PRINT_VARIABLE_INFO(main); // lvalue

    std::string text;
    PRINT_VARIABLE_INFO(text); // lvalue
    PRINT_VARIABLE_INFO(text + ""); // prvalue
    PRINT_VARIABLE_INFO(std::move(text + "")); // xvalue

    return 0;
}