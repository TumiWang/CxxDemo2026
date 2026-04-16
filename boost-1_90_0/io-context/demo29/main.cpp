#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/asio.hpp>

void add_task(boost::asio::io_context& context) {
    auto timer = std::make_shared<boost::asio::steady_timer>(context);
    std::cout << "timer的引用计数: " << timer.use_count() << std::endl;

    timer->expires_after(std::chrono::milliseconds(100));
    auto start = std::chrono::high_resolution_clock::now();
    timer->async_wait([start, timer](const boost::system::error_code& error) {
        // 注意: timer 一定要值捕获
        std::cout << "timer的引用计数: " << timer.use_count() << std::endl;
        // 这里, 因为都在值捕获, 也可以使用 通用值捕获= 例如
        // [=](const boost::system::error_code& error) {}
        // 因代码中使用了 timer 所以，使用 = 时，会自动值捕获 timer
        // 为增加引用计数 会添加一句:
        // auto self = timer;

        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "timer 耗时: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << "毫秒"
            << " -- "
            << error.what()
            << std::endl;
    });
    std::cout << "timer的引用计数: " << timer.use_count() << std::endl;
}

int main() {
    boost::asio::io_context context;

    add_task(context);

    std::cout << "context.run() 开始" << std::endl;

    context.run();

    std::cout << "context.run() 完成" << std::endl;
    
    return 0;
}