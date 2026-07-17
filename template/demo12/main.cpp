#include <iostream>
#include <type_traits>

#define PRINT_VARIABLE_INFO(v)  \
    if (std::is_array<decltype(v)>::value) \
        std::cout << #v << " is array" << std::endl; \
    if (std::is_pointer<decltype(v)>::value) \
        std::cout << #v << " is pointer" << std::endl;

void f(int array[]) {
    PRINT_VARIABLE_INFO(array);
}

int main() {
    int a[] = {0, 1, 2, 3, 4};

    PRINT_VARIABLE_INFO(a);
    f(a);

    return 0;
}