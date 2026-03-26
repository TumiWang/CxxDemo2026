#include <functional>
#include <iostream>

#include <boost/asio.hpp>

void f1() {
    std::cout << "f1" << std::endl;
}

void f2(int a) {
    std::cout << "f2 -- " << a << std::endl;
}

int f3() {
    std::cout << "f3" << std::endl;
    return 3;
}

int main() {
    boost::asio::io_context context;

    boost::asio::post(context, f1);

    // 下面这个句, 会编译失败 -- 没有参数
    // boost::asio::post(context, f2, 1);

    // 下面这句没有问题
    boost::asio::post(context, std::bind(f2, 1));

    // 下面这句 可以编译成功，但 f3的返回值会被忽略
    boost::asio::post(context, f3);

    context.run();

    return 0;
}