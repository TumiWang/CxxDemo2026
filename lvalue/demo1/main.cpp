#include <chrono>
#include <iostream>
#include <string>
#include <type_traits>

// 下面这组3个是判断是否为 prvalue
template<class T>
struct is_prvalue: std::true_type {};
template<class T>
struct is_prvalue<T&>: std::false_type {};
template<class T>
struct is_prvalue<T&&>: std::false_type {};

// 下面这组3个是判断是否为 xvalue
template<class T>
struct is_xvalue: std::false_type {};
template<class T>
struct is_xvalue<T&>: std::false_type {};
template<class T>
struct is_xvalue<T&&>: std::true_type {};

// 下面这组3个是判断是否为 lvalue
template<class T>
struct is_lvalue: std::false_type {};
template<class T>
struct is_lvalue<T&>: std::true_type {};
template<class T>
struct is_lvalue<T&&>: std::false_type {};

// 上面9个 在使用上和 type_traits头文件提供的有一个差别
// 例如：
// int a;
// 上面的获取类型要使用
// decltype((a))
// type_traits头文件提供 通常都是
// decltype(a)
//    多一组小括弧
#define PRINT_VARIABLE_INFO(v)  \
    if (is_prvalue<decltype((v))>::value)   \
        std::cout << #v << " is a prvalue" << std::endl; \
    if (is_xvalue<decltype((v))>::value)   \
        std::cout << #v << " is a xvalue" << std::endl; \
    if (is_lvalue<decltype((v))>::value)   \
        std::cout  << #v << " is a lvalue" << std::endl;

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

    PRINT_VARIABLE_INFO(&v0); // lvalue
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