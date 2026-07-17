#include <iostream>

void f(int (&array)[5]) {
    for(size_t i = 0; i < sizeof(array) / sizeof(array[0]); ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    int a[] = {0, 1, 2, 3, 4};
    f(a);

    // 下面这个编译失败 因为 b 是 int b[6]
    // 这样找不到对应的 函数f
    // int b[] = {0, 1, 2, 3, 4, 5};
    // f(b);

    return 0;
}