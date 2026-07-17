#include <iostream>

template<size_t N>
void f(int (&array)[N]) {
    for(size_t i = 0; i < N; ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

template<class T, size_t N>
void f(T (&array)[N]) {
    for(size_t i = 0; i < N; ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    int a[] = {0, 1, 2, 3, 4};
    f(a);

    int b[] = {0, 1, 2, 3, 4, 5};
    f(b);

    return 0;
}