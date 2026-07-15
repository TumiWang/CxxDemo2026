// 文件 main.cpp

#include "math.h"

int main() {
    auto a = add(1, 2);

    // 下面这句 会有链接问题
    // auto b = add(1.0f, 2.0f);

    return 0;
}