#include <cfenv>
#include <cmath>
#include <iomanip>
#include <iostream>

int main()
{
    std::cout << std::setprecision(50);

    std::cout << "default:" << std::endl;
    std::cout << " std::stof(\"1.1\"): "
              << std::stof("1.1") << std::endl;
    std::cout << "std::stof(\"-1.1\"): "
              << std::stof("-1.1") << std::endl;
    std::cout << std::endl;

    std::cout << "std::stof(\"1.1\"): " << std::endl;
    std::fesetround(FE_DOWNWARD);
    std::cout << "     FE_DOWNWARD: "
              << std::stof("1.1") << std::endl;
    std::fesetround(FE_UPWARD);
    std::cout << "       FE_UPWARD: "
              << std::stof("1.1") << std::endl;
    std::fesetround(FE_TONEAREST);
    std::cout << "    FE_TONEAREST: "
              << std::stof("1.1") << std::endl;
    std::fesetround(FE_TOWARDZERO);
    std::cout << "   FE_TOWARDZERO: "
              << std::stof("1.1") << std::endl;
    std::cout << std::endl;

    std::cout << "std::stof(\"-1.1\"):" << std::endl;
    std::fesetround(FE_DOWNWARD);
    std::cout << "      FE_DOWNWARD: "
              << std::stof("-1.1") << std::endl;
    std::fesetround(FE_UPWARD);
    std::cout << "        FE_UPWARD: "
              << std::stof("-1.1") << std::endl;
    std::fesetround(FE_TONEAREST);
    std::cout << "     FE_TONEAREST: "
              << std::stof("-1.1") << std::endl;
    std::fesetround(FE_TOWARDZERO);
    std::cout << "    FE_TOWARDZERO: "
              << std::stof("-1.1") << std::endl;
    return 0;
}