#include <cfenv>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>

#pragma STDC FENV_ACCESS ON

class FeException final {
public:
    FeException(const char* info, bool high_precision, std::function<float()> proc)
      : info_(info), high_precision_(high_precision), proc_(proc) {

    }
    ~FeException() {
        std::feclearexcept(FE_ALL_EXCEPT);

        if (high_precision_) std::cout << std::setprecision(50);
        else std::cout << std::setprecision(6);

        std::cout << info_ << " = " << proc_() << std::endl;

        if (std::fetestexcept(FE_DIVBYZERO))
            std::cout << info_ << " ---- 分母不能为0" << std::endl;
        if (std::fetestexcept(FE_INEXACT))
            std::cout << info_ << " ---- 计算前先进行了舍入操作" << std::endl;
        if (std::fetestexcept(FE_INVALID))
            std::cout << info_ << " ---- 参数无效" << std::endl;
        if (std::fetestexcept(FE_OVERFLOW))
            std::cout << info_ << " ---- 计算越界" << std::endl;
        if (std::fetestexcept(FE_UNDERFLOW))
            std::cout << info_ << " ---- 精度丢失" << std::endl;
        
        if (!std::fetestexcept(FE_ALL_EXCEPT))
            std::cout << info_ << " ---- 未发生问题" << std::endl;
        std::cout << std::endl;
    }

private:
    const char* info_;
    bool high_precision_;

    std::function<float()> proc_;
};

union FloatUnit
{
    unsigned char buf[4];
    float v;
};

int main()
{
    float zero = 0.f;
    float one = 1.f;
    float two = 2.f;

    FloatUnit a = { .buf = {0xff, 0xff, 0xff, 0x7e}};
    FloatUnit b = { .buf = {0x1e, 1, 0, 0}};
    FloatUnit c = { .buf = {0x1f, 1, 0, 0}};

    std::cout << std::fixed;

    // 定义的临时变量，因为没有变量名，
    // 所以，构造后会立即析构
    FeException("1 / 0", false, [&] {
        return one / zero;
    });

    std::cout << "    a = " << a.v << std::endl;
    std::cout << std::endl;

    // 因为 a 的值太大了，导致 a + 1 等于 a
    FeException("a + 1", false, [&] {
        return a.v + one;
    });

    FeException("a + a", false, [&] {
        return a.v + a.v;
    });

    // a + a 和 a * 2 的表现一致, 这里不展示了
    // FeException("a * 2", false, [&] {
    //     return a.v * two;
    // });

    FeException("a + a + a", false, [&] {
        return a.v + a.v + a.v;
    });

    std::cout << std::setprecision(50);
    std::cout << "    b = " << b.v << std::endl;

    // b / 2 不会精度丢失
    FeException("b / 2", true, [&] {
        return b.v / two;
    });

    std::cout << std::setprecision(50);
    std::cout << "    c = " << c.v << std::endl;

    // c / 2 会精度丢失
    FeException("c / 2", true, [&] {
        return c.v / two;
    });

    // 在实数的范围内 这是无效的
    FeException("sqrt(-1)", false, [&] {
        return std::sqrt(-1.f);
    });

    return 0;
}