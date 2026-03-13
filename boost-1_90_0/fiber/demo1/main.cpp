#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
// #include <thread>

#include <boost/fiber/all.hpp>

class StatisticsDuration final
{
public:
    StatisticsDuration(const std::string& prefix)
        : prefix_(prefix) {
        start_ = std::chrono::high_resolution_clock::now();
    }
    ~StatisticsDuration() {
        auto end = std::chrono::high_resolution_clock::now();
        const std::chrono::duration<double> duration = end - start_;

        std::ostringstream os;
        os << prefix_ << " ---- " << duration.count() << "秒";

        std::cout << os.str() << std::endl;
    }

private:
    const std::string prefix_;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};

void DoSomething(const std::string& info) {
    StatisticsDuration d(info);

    // std::this_thread::sleep_for(std::chrono::seconds(5));
    boost::this_fiber::sleep_for(std::chrono::seconds(5));
}

int main()
{
    StatisticsDuration d("总耗时");

    DoSomething("任务1");
    DoSomething("任务2");

    return 0;
}