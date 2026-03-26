#include <iostream>
#include <thread>

#include <boost/asio.hpp>

int main() {
    std::cout << "主线程: " << std::this_thread::get_id() << " -- 开始" << std::endl;

    boost::asio::io_context context;

    boost::asio::post(context, [&]{
        std::cout << "任务1 所在线程: " << std::this_thread::get_id() << std::endl;

        boost::asio::post(context, []{
            std::cout << "任务2 所在线程: " << std::this_thread::get_id() << std::endl;
        });
    });

    std::thread t([&]{
        std::cout << "工作线程: " << std::this_thread::get_id() << " -- 开始" << std::endl;

        context.run();

        std::cout << "工作线程: " << std::this_thread::get_id() << " -- 完成" << std::endl;
    });

    std::cout << "主线程: " << std::this_thread::get_id() << " -- 等待工作线程完成" << std::endl;

    t.join();

    std::cout << "主线程: " << std::this_thread::get_id() << " -- 完成" << std::endl;

    return 0;
}