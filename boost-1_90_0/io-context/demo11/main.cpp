#include <chrono>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <boost/asio.hpp>

// 线程池中的线程数量
#define THREAD_POOL_CAPACITY  3

// 线程安全的方式访问 std::cout
void AppendLineToCount(const std::string& text) {
    static std::mutex cout_mutex;

    std::lock_guard<decltype(cout_mutex)> lock(cout_mutex);
    std::cout << text << std::endl;
}

// 向 context 添加 count + 1 个 任务
// count 为 0 时，任务是停止 context
void defer_task(boost::asio::io_context& context, int count) {
    if (count > 0) {
        // 注意: 下面lamdba 的两个捕获参数的不同方式
        boost::asio::post(context, [count,&context]{
            defer_task(context, count - 1);

            std::ostringstream os;
            os << "工作线程: " << std::this_thread::get_id()
                << " -- 任务" << count;
            AppendLineToCount(os.str());
            }
        );
    } else {
        // 这里使用一个延迟的关闭任务
        // 这是为了避免关闭时，队列中还有待执行中的任务
        // 注意: 只会导致待执行的任务被抛弃，不影响正在执行的任务
        // 而这里根据使用的方式实际上在运行到这里时，不太可能有待运行的任务
        // 所以，这里实际上可以直接使用 context.stop();
        // 但可能有问题
        // 但使用下面的不会有问题:
        // boost::asio::defer(context, [&]{context.stop();});
        boost::asio::steady_timer timer(context);
        timer.expires_after(std::chrono::milliseconds(100));
        timer.async_wait(
            // 该任务是在调用timer.async_wait的 100毫秒后 触发
            // 或者可以认为是在调用timer.async_wait的 100毫秒后, 才进入任务队列
            //     -- 这样虽然不准确，但对达到的效果更好理解
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
    boost::asio::io_context context(THREAD_POOL_CAPACITY);

    std::vector<std::thread> thread_pool;

    auto work = boost::asio::make_work_guard(context);

    thread_pool.reserve(THREAD_POOL_CAPACITY);
    for (unsigned int i = 0; i < THREAD_POOL_CAPACITY; ++i) {
        thread_pool.emplace_back([&]{
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

    // sleep 确保所有工作线程都运行 -- 在实际中没必要
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // 向队列以类似递归的方式添加任务
    // 任务的数量为 THREAD_POOL_CAPACITY * 2 + 1 个
    defer_task(context, THREAD_POOL_CAPACITY * 2);
    
    // 等待所有工作线程退出
    for (auto& thread: thread_pool) {
        if (thread.joinable()) thread.join();
    }
    
    return 0;
}