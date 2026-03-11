#include <iostream>

int main()
{
    float a = 1.f;
    float b = -1.f;
    float c = 0.f;
    
    std::cout << "1.0 / 0.0 = " << a / c << std::endl;
    std::cout << "-1.0 / 0.0 = " << b / c << std::endl;

    return 0;
}