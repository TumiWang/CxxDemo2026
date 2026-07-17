// 文件 main.cpp

#include <iostream>
#include <type_traits>

// template<class T, std::enable_if_t<std::is_array<T>::value>>
    template<class T>
void f(T array) {
    for(size_t i = 0; i < std::extent<T>::value; ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    int a[] = {0, 1, 2, 3, 4};

    f(a);

    return 0;
}