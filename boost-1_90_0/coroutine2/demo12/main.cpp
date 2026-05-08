#include <iostream>

#include <boost/coroutine2/all.hpp>

using CoroutinePullType = boost::coroutines2::coroutine<void>::pull_type;
using CoroutinePushType = boost::coroutines2::coroutine<void>::push_type;

void foo(CoroutinePushType& sink) {
    std::cout << "a ";
    sink();
    std::cout << "b ";
    sink();
    std::cout << "c ";
    sink();
}

void bar(CoroutinePullType& sink) {
    std::cout << "1 ";
    sink();
    std::cout << "2 ";
    sink();
    std::cout << "3 ";
    sink();
}

int main()
{
    CoroutinePullType pull([](CoroutinePushType& sink){
        foo(sink);
    });
    bar(pull);

    // 上面的代码和下面这段效果一致
    // CoroutinePushType push([](CoroutinePullType& sink){
    //     bar(sink);
    // });
    // foo(push);

    // 最后补充一个换行
    std::cout << std::endl;

    return 0;
}