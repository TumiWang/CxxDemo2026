// 同 #include <string.h>
// 通常 memcpy 在该头文件中定义
#include <cstring>

#include <iostream>

// 这个是小端的内存描述
template<class T>
void PrintMemoryInfo(const T& v) {
    char buff[sizeof(T)] = { 0 };

    memcpy(buff, &v, sizeof(T));

    for (int i = sizeof(T) - 1; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {
            if (buff[i] & (1 << j)) std::cout << "1";
            else std::cout << "0";
        }
        if (i != 0) std::cout << " ";
        else std::cout << std::endl;
    }
}

int main() {
    std::cout << "        整数 1:" << std::endl;
    PrintMemoryInfo(1); // int

    std::cout << "单精度浮点数 -1:" << std::endl;
    PrintMemoryInfo(-1.f); // float

    std::cout << "单精度浮点数 1:" << std::endl;
    PrintMemoryInfo(1.f);

    std::cout << "双精度浮点数 1:" << std::endl;
    PrintMemoryInfo(1.0); // double

    std::cout << "单精度浮点数 0.25:" << std::endl;
    PrintMemoryInfo(0.25f);

    std::cout << "双精度浮点数 0.25:" << std::endl;
    PrintMemoryInfo(1.25f);

    std::cout << "单精度浮点数 1.1:" << std::endl;
    PrintMemoryInfo(1.1f);

    std::cout << "双精度浮点数 1.1:" << std::endl;
    PrintMemoryInfo(1.1);

    return 0;
}