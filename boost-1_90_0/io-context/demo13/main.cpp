#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

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
    // 在其它线程没有启动前，访问 std::cout
    // 这样, 确保 不会并发的访问 std::cout
    std::cout << "主线程: " << std::this_thread::get_id() << std::endl;

    auto work_cout = boost::asio::make_work_guard(cout_context_);
    std::thread([]{
        std::cout << "std::cout所在线程: " << std::this_thread::get_id()
                  << " -- 开始" << std::endl;
        cout_context_.run();
    }).detach();

    boost::asio::system_executor executor1;
    boost::asio::system_executor executor2;

    boost::asio::post(executor1, [&]{
        OutputInfo("任务1");
        boost::asio::post(executor1, []{
            OutputInfo("任务2");
        });
    });

    boost::asio::post(executor2, [&]{
        OutputInfo("任务3");
    });

    boost::asio::post(executor2, [&]{
        OutputInfo("任务4");
    });

    // 如果这里不等一会 ---- 100毫秒
    // 大概率 4个任务不能全部执行就会退出了
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    return 0;
}