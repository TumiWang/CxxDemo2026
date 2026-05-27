
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#endif 

#include "boost/asio.hpp"
#include "boost/nowide/iostream.hpp"
#include "boost/process.hpp"

#include <functional>

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
    boost::asio::readable_pipe pipe(context);
    boost::process::process proc(context.get_executor(),
        ping_filename,
#ifdef _WIN32
        {"-t", "baidu.com"},
#else
        {"baidu.com"},
#endif
        boost::process::process_stdio{nullptr, pipe, nullptr}
    );

    // 程序PING的pipe读缓存
    char buffer[1024] = { 0 };
    // 支持递归调用的读pipe -- Lamdba
    // 注: 这时类型不能使用auto，要使用 std::function 明确参数和返回值
    std::function<void()> async_read_all_from_pipe = [&]() -> void {
        // buffer填0
        memset(buffer, 0, sizeof(buffer));
        pipe.async_read_some(boost::asio::buffer(buffer, sizeof(buffer)),
            [&](boost::system::error_code ec, int count) {
                if (ec) return;
                
                boost::nowide::cout << "PING结果: ";
                boost::nowide::cout.flush();
                std::cout << buffer;
                std::cout.flush();
                
                // 继续异步读
                async_read_all_from_pipe();
            }
        );
    };
    // 开始读pipe -- 内部递归
    async_read_all_from_pipe();

    auto task1 = boost::process::async_execute(std::move(proc));
    auto task2 = std::move(task1)(boost::asio::cancel_after(std::chrono::seconds(100), boost::asio::cancellation_type::total));
    std::move(task2)(boost::asio::detached);

    context.run();

    return 0;
}