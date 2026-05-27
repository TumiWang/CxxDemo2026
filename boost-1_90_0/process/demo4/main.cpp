
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

#ifdef _WIN32
    boost::asio::signal_set signals(context, SIGINT);
#endif

    // 创建进程
    boost::process::popen proc(context.get_executor(),
        ping_filename,
#ifdef _WIN32
        {"-t", "baidu.com"}
#else
        {"baidu.com"}
#endif
    );

    // 程序PING的输出结果处理
    auto proc_ping_info = [&proc](boost::system::error_code& ec) {
        char buffer[512] = { 0 };
        // boost::process::popen类支持
        // 1. 同步写方法 ---- write_some
        // 2. 异步写方法 ---- async_write_some
        // 3. 同步读方法 ---- read_some
        // 4. 异步读方法 ---- async_read_some
        // 这里使用的是 同步读方法
        proc.read_some(boost::asio::buffer(buffer, sizeof(buffer)), ec);

        if (ec) return;

        boost::nowide::cout << "PING结果: ";
        boost::nowide::cout.flush();

        std::cout << buffer;
        std::cout.flush();
    };

    boost::system::error_code ec;

    // 从PING程序中读取5次输出信息
    for (int i = 0; i < 5; ++i) {
        proc_ping_info(ec);
        if (ec) return 1; // 读取失败则return
    }

    boost::nowide::cout << "\n即将中断PING程序" << std::endl;
    // 向PING程序发送中断
    proc.interrupt(ec);

    // 从PING程序读取后续的所有信息
    for (;;) {
        proc_ping_info(ec);
        if (ec == boost::asio::error::eof) break;
        // boost::asio::error::eof 表示已经到结尾了
        // boost::asio::error::eof 也是错误 即也满足 if(ec)
        //   所以，要先判断 if (ec == boost::asio::error::eof)
        if (ec) return 1;
    }

    return 0;
}