
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#endif 

#include "boost/asio.hpp"
#include "boost/nowide/iostream.hpp"
#include "boost/process.hpp"

int main() {
    auto ping_filename = boost::process::environment::find_executable("ping");

    if (ping_filename.empty()) {
        boost::nowide::cout << "没有找到程序: ping" << std::endl;
        return 1;
    }
    
    boost::asio::io_context context;
    boost::process::process proc(context.get_executor(),
        ping_filename,
#ifdef _WIN32
        {"-t", "baidu.com"}
#else
        {"baidu.com"}
#endif
    );

    // 把进程 proc 封装为一个异步的任务 task1
    // 实际上, 称 task1 是任务并不准确；它实际上是创建任务的方法
    // 可以认为它是一个任务的创建方法
    auto task1 = boost::process::async_execute(std::move(proc));

    // 使用创建任务的方法 task1
    // 创建任务 task2 -- 20秒后中断进程 proc
    //     中断进程    -- boost::asio::cancellation_type::total
    //     请求退出进程 -- boost::asio::cancellation_type::request_exit
    //     终止进程    -- boost::asio::cancellation_type::terminal
    // 实际上 使用 task1 创建了一个延时任务 -- 10秒后中断(ping)进程
    // 并生成一个新的任务创建方法 -- task2
    //   下面代码执行后 task1已经无效了, 所以生成 task2
    auto task2 = std::move(task1)(boost::asio::cancel_after(std::chrono::seconds(10), boost::asio::cancellation_type::total));

    // 实际上，执行在这里时, context上并没有带运行的任务

    // 创建任务 进程proc退出时的任务 -- 这里使用的是detached的描述
    std::move(task2)(boost::asio::detached);

    // 这时, 才真的有任务在 context 上
    
    context.run();

    return 0;
}