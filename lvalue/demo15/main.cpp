#include <iostream>
#include <type_traits>

template<int N1, int N2>
struct S
{
    const int v = N1 + N2;
};

template<int N>
struct S<2, N>
{
    const int v = N * 20;
};
template<int N>
struct S<N, 0>
{
    const int v = N * 10;
};

#define PRINT_VARIABLE_INFO(v)  \
    std::cout << #v << " = " << v << std::endl;

int main() {
    S<1, 2> s1; // v = 1 + 2;
    S<2, 1> s2; // v = 1 * 20;
    // S<2, 0> s3; // v = 0 * 20;

    PRINT_VARIABLE_INFO(s1.v);
    PRINT_VARIABLE_INFO(s2.v);
    // PRINT_VARIABLE_INFO(s3.v);

    return 0;
}