#include "math.h"

template<class T>
T add(T a, T b) {
    return a + b;
}

// 这句很重要
// 下面这句保证 
// add(1, 0) 
// 能够链接
template int add(int, int);