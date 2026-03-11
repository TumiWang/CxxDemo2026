#include <cfenv>
#include <iostream>

// 
#pragma STDC FENV_ACCESS ON

int main()
{
    float a = 1.f;
    float b = 0.f;
    
    std::feclearexcept(FE_ALL_EXCEPT);
    std::cout << "0.0 / 1.0 = " << b / a << std::endl;
    if (std::fetestexcept(FE_ALL_EXCEPT)) std::cout << "0.0 / 1.0 -- 计算时发生异常" << std::endl;

    std::feclearexcept(FE_ALL_EXCEPT);
    std::cout << "1.0 / 0.0 = " << a / b << std::endl;
    if (std::fetestexcept(FE_ALL_EXCEPT)) std::cout << "1.0 / 0.0 -- 计算时发生异常" << std::endl;
    if (std::fetestexcept(FE_DIVBYZERO)) std::cout << "1.0 / 0.0 -- 除数为零" << std::endl;

    return 0;
}