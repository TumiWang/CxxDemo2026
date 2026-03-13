#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

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

    boost::this_fiber::sleep_for(std::chrono::seconds(5));
}

int main()
{
    // boost::fibers::use_scheduling_algorithm<boost::fibers::algo::round_robin>();

    StatisticsDuration d("总耗时");

    std::vector<boost::fibers::fiber> fibers;

    fibers.emplace_back([]{
        DoSomething("任务1");
    });

    fibers.emplace_back([]{
        DoSomething("任务2");
    });

    for (auto& fiber: fibers) {
        fiber.join();
    }

    return 0;
}