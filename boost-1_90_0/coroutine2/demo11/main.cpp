#include <iostream>
#include <vector>

#include <boost/fiber/all.hpp>

// 该函数会拦截  std::cout << "" 类似的执行
// 该定义一定要在 foo 和 bar 的实现代码之前
decltype(std::cout)& operator<<(decltype(std::cout)& os, const char* text) {
    // 下面是 代替 os << text 避免拦截递归死循环
    for (unsigned int i = 0; *(text + i) != '\0'; ++i) {
        os.put(os.widen(*(text + i)));
    }

    // 执行 os << text 后, sleep一下，达到切换的目的
    boost::this_fiber::sleep_for(std::chrono::milliseconds(10));
    return os;
}

void foo() {
    std::cout << "a ";
    std::cout << "b ";
    std::cout << "c ";
}

void bar() {
    std::cout << "1 ";
    std::cout << "2 ";
    std::cout << "3 ";
}

int main()
{
    // 创建两个纤程 分别用来执行 foo 和 bar
    std::vector<boost::fibers::fiber> fibers;
    fibers.emplace_back([]{
        foo();
    });
    fibers.emplace_back([]{
        bar();
    });
    for (auto& fiber: fibers) {
        fiber.join();
    }

    // 最后补充一个换行
    std::cout << std::endl;

    return 0;
}