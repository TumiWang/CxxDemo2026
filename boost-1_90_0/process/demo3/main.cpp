
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#endif 

#include "boost/asio.hpp"
#include "boost/nowide/iostream.hpp"
#include "boost/process.hpp"

#include <iostream>
#include <thread>

int main() {
    auto ping_filename = boost::process::environment::find_executable("ping");

    if (ping_filename.empty()) {
        boost::nowide::cout << "没有找到程序: ping" << std::endl;
        return 1;
    }
    
    boost::asio::io_context context;

    // 读操作的通讯管道
    boost::asio::readable_pipe pipe(context);

    // 创建进程
    boost::process::process proc(context.get_executor(),
        ping_filename,
#ifdef _WIN32
        {"-t", "baidu.com"},
#else
        {"baidu.com"},
#endif
        // 这里有三个参数
        // 第一个表示标准输入
        // 第二个表示标准输出
        // 第三个表示错误输出
        // 这里放弃了标准输入和错误输出
        // 并使用 管道 pipe 接到了标准输出上
        boost::process::process_stdio{nullptr, pipe, nullptr}
    );

#ifdef _WIN32
    // 定义变量 signals, 表示这个变量内处理 SIGINT
    // 在Windows上, SIGINT 通常就是Ctrl+C，这个消息
    boost::asio::signal_set signals(context, SIGINT);
    // 但实际上我们并不真的要处理 SIGINT, 只是要拦截SIGINT
    // 所以，并没有设置 SIGINT 的处理方法
    // 下面注释掉的是处理 SIGINT 的方法
    // signals.async_wait([](boost::system::error_code ec, int sig){
    //     // 参数 ec 表示发生的错误
    //     // sig 表示收到的 signal --- 这里只监听了 SIGINT
    // });
#endif

    // 创建一个负责IO的工作线程
    std::thread io_thread([&]{
        for(;;) {
            char buffer[1024] = { 0 };
            boost::system::error_code ec;

            auto r = pipe.read_some(boost::asio::buffer(buffer, sizeof(buffer)), ec);

            if (ec) break; // ec表示失败时 跳出循环

            boost::nowide::cout << "PING结果: ";
            boost::nowide::cout.flush();

            // 这里的buffer中的内容，来源于另一个程序ping
            // 我们不清楚ping程序输出的编码方式，但它更大的可能是和std::cout一致
            // 这点在Windows上很重要
            std::cout << buffer;
            std::cout.flush();
        }
    });

    auto task1 = boost::process::async_execute(std::move(proc));
    auto task2 = std::move(task1)(boost::asio::cancel_after(std::chrono::seconds(10), boost::asio::cancellation_type::total));
    std::move(task2)(boost::asio::detached);

    context.run();

    // 释放 io_thread
    io_thread.join();

    return 0;
}