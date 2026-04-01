#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <boost/asio.hpp>

// 线程池中的线程数量
#define THREAD_POOL_CAPACITY  3

boost::asio::io_context cout_context_;

void AppendLineToCount(const std::string& text) {
    boost::asio::post(cout_context_, [text]{
        std::cout << text << std::endl;
    });
}

void defer_task(boost::asio::io_context& context, int count) {
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
    auto work_cout = boost::asio::make_work_guard(cout_context_);
    // 启动一个工作线程处理 cout_context_的任务
    // 这样，实际上的访问 std::cout 
    //     ---- 即：对 std::cout 的写操作
    // 于是，只有这个工作线程使用 std::cout，
    // 这样自然不需要锁保护 std::cout
    std::thread([]{
        cout_context_.run();
    }).detach();

    boost::asio::io_context context(THREAD_POOL_CAPACITY);

    std::vector<std::thread> thread_pool;

    auto work = boost::asio::make_work_guard(context);

    thread_pool.reserve(THREAD_POOL_CAPACITY);
    for (unsigned int i = 0; i < THREAD_POOL_CAPACITY; ++i) {
        thread_pool.emplace_back([&context]{
            {
                std::ostringstream os;
                os << "工作线程: " << std::this_thread::get_id() << " -- 开始";
                AppendLineToCount(os.str());
            }

            context.run();

            {
                std::ostringstream os;
                os << "工作线程: " << std::this_thread::get_id() << " -- 结束";
                AppendLineToCount(os.str());
            }
        });
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    defer_task(context, THREAD_POOL_CAPACITY * 2);
    
    for (auto& thread: thread_pool) {
        if (thread.joinable()) thread.join();
    }
    
    return 0;
}