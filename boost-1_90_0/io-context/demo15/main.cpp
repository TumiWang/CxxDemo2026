#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <boost/asio.hpp>

boost::asio::io_context cout_context_;

void OutputInfo(const std::string& text) {
    std::ostringstream os;
    os << "工作线程: " << std::this_thread::get_id()
       << " -- " << text;
    const std::string& content = os.str();
    boost::asio::post(cout_context_, [content]{
        std::cout << content << std::endl;
    });
}

int main() {
    auto work_cout = boost::asio::make_work_guard(cout_context_);
    std::thread cout_thread([]{
        cout_context_.run();
    });

    // 这次有3个任务，线程池中创建3个线程
    boost::asio::thread_pool thread_pool(3);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    boost::asio::post(thread_pool, []{
        OutputInfo("任务1: 开始");
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        OutputInfo("任务1: 结束");
    });

    {
        // 这里是限制了 strand 的生命周期 -- 但实际上没有差别
        auto strand = boost::asio::make_strand(thread_pool);

        boost::asio::post(strand, []{
            OutputInfo("任务2: 开始");
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
            OutputInfo("任务2: 结束");
        });

        boost::asio::post(strand, []{
            OutputInfo("任务3: 开始");
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
            OutputInfo("任务3: 结束");
        });
    }

    // 等100毫秒，确保任务能执行完成
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // 明确的停止 处理 cout_context_ 的工作线程
    work_cout.reset();
    cout_thread.join();
    
    return 0;
}