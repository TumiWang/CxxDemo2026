#include <iostream>
#include <string>

template<class T>
void f(T&& text) {
    if (std::is_lvalue_reference<decltype(text)>::value)
        std::cout << "function f 参数text是左值引用" << std::endl;
    else if (std::is_rvalue_reference<decltype(text)>::value)
        std::cout << "function f 参数text是右值引用" << std::endl;

    std::cout << "function f -- " << text << std::endl;
}

template<class T>
void g(T&& text) {
    if (std::is_lvalue_reference<decltype(text)>::value)
        std::cout << "function g 参数text是左值引用" << std::endl;
    else if (std::is_rvalue_reference<decltype(text)>::value)
        std::cout << "function g 参数text是右值引用" << std::endl;

    f(std::move(text));
}

int main() {
    std::string text = "test";

    g(text);

    std::cout << "============" << std::endl;

    g(std::move(text));

    return 0;
}