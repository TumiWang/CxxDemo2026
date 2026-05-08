#include <iostream>

#include <boost/coroutine2/all.hpp>

int main()
{
    boost::coroutines2::coroutine<int>::pull_type fibonacci(
        [](boost::coroutines2::coroutine<int>::push_type& sink) {
            int first = 1;
            int second = 1;
            sink(first);
            sink(second);
            for (int i = 2; i < 8; ++i) {
                int temp = first + second;
                first = second;
                second = temp;
                sink(second);
            }
        }
    );

    for(auto v : fibonacci) {
        std::cout << v << ",";
    }
    std::cout << std::endl;

    return 0;
}