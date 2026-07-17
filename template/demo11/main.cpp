#include <iostream>

void f(int array[]) {
    for(size_t i = 0; i < sizeof(array) / sizeof(int); ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    int a[] = {0, 1, 2, 3, 4};

    f(a);

    return 0;
}