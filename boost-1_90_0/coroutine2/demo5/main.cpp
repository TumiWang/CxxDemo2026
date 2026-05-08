#include <ctime>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include <boost/coroutine2/all.hpp>

// 使用这个函数对数据进行一次转换，主要是增加生产数据的开销
// 但实际上，这个开销还不够
int transfor(int n) {
    return (n + 2) * (n + 1) * n;
}

// 这个就相对于高级语言的生成器
boost::coroutines2::coroutine<int>::pull_type GetDataWithCoroutine2(unsigned int count) {
    boost::coroutines2::coroutine<int>::pull_type generator(
        [count](boost::coroutines2::coroutine<int>::push_type& sink) {
            for (int i = 1; i <= count; ++i) {
                sink(transfor(i));
            }
        }
    );

    return generator;
}

std::vector<int> GetDataWithVector(unsigned int count) {
    std::vector<int> v;
    v.reserve(count);

    for (int i = 1; i <= count; ++i) {
        v.push_back(transfor(i));
    }

    return v;
}

class Log final
{
public:
    Log(const std::string& prefix) 
        : start_(std::clock()), prefix_(prefix) {}
    ~Log() {
        const std::clock_t now = std::clock();
        std::cout << prefix_ << " CPU耗时: "
            << 1000000.0 * (now - start_) / CLOCKS_PER_SEC << std::endl;
    }

private:
    const std::clock_t start_;
    const std::string prefix_;
};

int main()
{
    const unsigned int items_count = 10000;
    const unsigned int break_count = items_count / 20;

    {
        Log log("全容器数据");
        auto items = GetDataWithVector(items_count);
        int count = 0;
        for(auto iter = items.begin(); iter != items.end(); ++iter, ++count)
            if (count == items_count) break;
    }

    {
        Log log("5%容器数据");
        auto items = GetDataWithVector(items_count);
        int count = 0;
        for(auto iter = items.begin(); iter != items.end(); ++iter, ++count)
            if (count == break_count) break;
    }

    {
        Log log("全生成器数据");
        auto items = GetDataWithCoroutine2(items_count);
        int count = 0;
        for(auto iter = items.begin(); iter != items.end(); ++iter, ++count)
            if (count == items_count) break;
    }

    {
        Log log("5%生成器数据");
        auto items = GetDataWithCoroutine2(items_count);
        int count = 0;
        for(auto iter = items.begin(); iter != items.end(); ++iter, ++count)
            if (count == break_count) break;
    }

    return 0;
}