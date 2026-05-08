#include <iostream>

#include <boost/coroutine2/all.hpp>

using CoroutinePullType = boost::coroutines2::coroutine<void>::pull_type;
using CoroutinePushType = boost::coroutines2::coroutine<void>::push_type;

int main()
{
    std::cout << "---------------- 1" << std::endl;
    {
        CoroutinePullType pull([](CoroutinePushType& sink){
            // 下面会执行
            std::cout << "Need to Push" << std::endl;
        });
    }
    std::cout << "---------------- 2" << std::endl;
    {
        CoroutinePushType push([](CoroutinePullType& sink){
            // 下面不会执行
            std::cout << "Need to Pull" << std::endl;
        });
    }
    std::cout << "---------------- 3" << std::endl;

    return 0;
}