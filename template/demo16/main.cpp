#include <iostream>
#include <type_traits>

int main() {
    int a[6][7][8] = { 0 };

    std::cout << std::rank<decltype(a)>::value << std::endl; // 3

    // std::extent<decltype(a)> 和
    // std::extent<decltype(a), 0>
    // 等效
    std::cout << std::extent<decltype(a), 0>::value << std::endl; // 6

    std::cout << std::extent<decltype(a), 1>::value << std::endl; // 7
    
    std::cout << std::extent<decltype(a), 2>::value << std::endl; // 8

    return 0;
}