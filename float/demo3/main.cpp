#include <iomanip>
#include <iostream>

int main()
{
    float a = 8.0f;
    float b = 0.000048f;
    float c = 0.00000048f;

    std::cout << std::setprecision(10) << std::fixed;

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "c = " << c << std::endl;

    std::cout << std::endl;

    std::cout << "a + b = " << a + b << std::endl;
    std::cout << "a + c = " << a + c << std::endl;
    std::cout << "b + c = " << b + c << std::endl;

    std::cout << std::endl;

    std::cout << "a + b - b = " << a + b - b << std::endl;

    return 0;
}