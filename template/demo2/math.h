#ifndef __math_h_
#define __math_h_

template<class T>
T add(T a, T b);

// 下面这句 通常不是必需，但可能对编译优化有好处
// extern template int add(int, int);

#endif // __math_h_