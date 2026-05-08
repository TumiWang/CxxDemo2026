#include <iostream>
#include <vector>

#include <boost/coroutine2/all.hpp>

// 这个就相对于高级语言的生成器
boost::coroutines2::coroutine<int>::pull_type GetDataWithCoroutine2(unsigned int count) {
    boost::coroutines2::coroutine<int>::pull_type generator(
        [count](boost::coroutines2::coroutine<int>::push_type& sink) {
            for (int i = 1; i <= count; ++i) {
                std::cout << "准备生成元素 -- " << i << std::endl;
                sink(i);
                std::cout << "已经生成元素 -- " << i << std::endl;
            }
        }
    );

    return generator;
}

std::vector<int> GetDataWithVector(unsigned int count) {
    std::vector<int> v;
    v.reserve(count);

    for (int i = 1; i <= count; ++i) {
        std::cout << "准备生成元素 -- " << i << std::endl;
        v.push_back(i);
        std::cout << "已经生成元素 -- " << i << std::endl;
    }

    return v;
}

int main()
{
    std::cout << "使用标准库容器 -- std::vector" << std::endl;
    {
        // 生命周期是为了控制 g1 释放时机
        auto g1 = GetDataWithVector(4);
        for (auto item: g1) {
            if (item == 3) {
                std::cout << "获取数据 -- " << item << " 不再继续迭代" << std::endl;
                break;
            } else {
                std::cout << "获取数据 -- " << item << std::endl;
            }
        }
    }

    std::cout << "----------------------" << std::endl;

    std::cout << "使用协程 -- boost::coroutine2" << std::endl;
    {
        // 生命周期是为了控制 g2 释放时机
        auto g2 = GetDataWithCoroutine2(4);
        for (auto item: g2) {
            if (item == 3) {
                std::cout << "获取数据 -- " << item << " 不再继续迭代" << std::endl;
                break;
            } else {
                std::cout << "获取数据 -- " << item << std::endl;
            }
        }
    }

    return 0;
}