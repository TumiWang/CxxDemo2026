#include <iostream>
#include <vector>

#include <boost/coroutine2/all.hpp>

template<class T>
class Log final
{
public:
    Log(const T& v)
        : v_(v) {
        std::cout << "Log构造 -- " << v_ << std::endl;
    }
    ~Log() {
        std::cout << "Log析构 -- " << v_ << std::endl;
    }

private:
    const T& v_;
};

// 这个就相对于高级语言的生成器
boost::coroutines2::coroutine<int>::pull_type GetDataWithCoroutine2(unsigned int count) {
    boost::coroutines2::coroutine<int>::pull_type generator(
        [count](boost::coroutines2::coroutine<int>::push_type& sink) {
            for (int i = 1; i <= count; ++i) {
                Log<decltype(i)> log(i);
                std::cout << "准备生成元素 -- " << i << std::endl;
                sink(i);
                std::cout << "已经生成元素 -- " << i << std::endl;
            }
        }
    );

    return generator;
}

int main()
{
    {
        auto data = GetDataWithCoroutine2(4);
        for (auto item: data) {
            if (item == 3) {
                std::cout << "获取数据 -- " << item << " 不再继续迭代" << std::endl;
                break;
            } else {
                std::cout << "获取数据 -- " << item << std::endl;
            }
        }
        std::cout << "data 准备释放" << std::endl;
    }
    std::cout << "data 已经释放" << std::endl;

    return 0;
}