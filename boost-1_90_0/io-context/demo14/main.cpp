#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <boost/asio.hpp>

// 线程池中的线程数量
#define THREAD_POOL_CAPACITY  3

void AppendLineToCount(const std::string& text) {
    static boost::asio::io_context cout_context;
    static auto work_cout = boost::asio::make_work_guard(cout_context);
    static std::once_flag cout_thread_flag;
    std::call_once(cout_thread_flag, [&]{
        std::thread([]{
            cout_context.run();
        }).detach();
    });

    boost::asio::post(cout_context, [text]{
        std::cout << text << std::endl;
    });
}

void defer_task(boost::asio::thread_pool& context, int count) {
    if (count > 0) {
        boost::asio::post(context, [count,&context]{
            defer_task(context, count - 1);

            std::ostringstream os;
            os << "工作线程: " << std::this_thread::get_id()
                << " -- 任务" << count;
            AppendLineToCount(os.str());
            }
        );
    } else {
        boost::asio::steady_timer timer(context);
        timer.expires_after(std::chrono::milliseconds(100));
        timer.async_wait(
            [count,&context](const boost::system::error_code& error_code) {
                context.stop();

                std::ostringstream os;
                os << "工作线程: " << std::this_thread::get_id()
                    << " -- 任务" << count;
                AppendLineToCount(os.str());
            }
        );
    }
}

int main() {
    boost::asio::thread_pool thread_pool(THREAD_POOL_CAPACITY);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    defer_task(thread_pool, THREAD_POOL_CAPACITY * 2);

    // 等100毫秒，确保任务能执行完成
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    return 0;
}