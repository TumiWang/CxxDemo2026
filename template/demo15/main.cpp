#include <iostream>
#include <type_traits>

template<typename T, typename std::enable_if<std::rank<T>::value, int>::type = 1>
void f(T& array) {
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