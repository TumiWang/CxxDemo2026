#include <iostream>

#include <boost/asio.hpp>

int main() {
    boost::asio::io_context context;

    boost::asio::defer(context, []{
        std::cout << "任务1" << std::endl;
    });

    boost::asio::dispatch(context, []{
        std::cout << "任务2" << std::endl;
    });

    boost::asio::post(context, [&]{
        std::cout << "任务3" << std::endl;
    });

    std::cout << "run 开始" << std::endl;

    context.run();

    std::cout << "run 完成" << std::endl;

    return 0;
}