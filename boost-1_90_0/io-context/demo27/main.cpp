#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/asio.hpp>

int main() {
    boost::asio::io_context context;

    {
        boost::asio::steady_timer timer(context);

        timer.expires_after(std::chrono::milliseconds(100));
        auto start = std::chrono::high_resolution_clock::now();
        timer.async_wait([start](const boost::system::error_code& error){
            auto end = std::chrono::high_resolution_clock::now();
            std::cout << "timer 耗时: "
                << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                << "毫秒"
                << " -- "
                << error.what()
                << std::endl;
        });

        // 在这里 timer 析构
        // 这会导致 timer.async_wait 的任务被取消
    }

    std::cout << "context.run() 开始" << std::endl;

    context.run();

    std::cout << "context.run() 完成" << std::endl;
    
    return 0;
}