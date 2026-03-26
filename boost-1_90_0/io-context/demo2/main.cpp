#include <iostream>

#include <boost/asio.hpp>

int main() {
    boost::asio::io_context context;

    boost::asio::post(context, [&]{
        std::cout << "任务1 -- 开始" << std::endl;

        boost::asio::post(context, []{
            std::cout << "任务5" << std::endl;
        });

        boost::asio::dispatch(context, []{
            std::cout << "任务6" << std::endl;
        });

        boost::asio::defer(context, []{
            std::cout << "任务7" << std::endl;
        });

        boost::asio::post(context, []{
            std::cout << "任务8" << std::endl;
        });

        boost::asio::defer(context, []{
            std::cout << "任务9" << std::endl;
        });

        boost::asio::post(context, []{
            std::cout << "任务10" << std::endl;
        });

        std::cout << "任务1 -- 完成" << std::endl;
    });

    boost::asio::dispatch(context, []{
        std::cout << "任务2" << std::endl;
    });

    boost::asio::defer(context, []{
        std::cout << "任务3" << std::endl;
    });

    boost::asio::post(context, []{
        std::cout << "任务4" << std::endl;
    });

    context.run();

    return 0;
}