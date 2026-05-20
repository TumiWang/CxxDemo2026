#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/asio.hpp>

int main() {
    boost::asio::io_context context;

    {
        boost::asio::steady_timer timer(context);

        timer.expires_after(std::chrono::milliseconds(100));
        timer.async_wait([](const boost::system::error_code& error){
            std::cout << "test" << std::endl;
        });
    }

    std::cout << "start" << std::endl;

    context.run();

    std::cout << "end" << std::endl;
    
    return 0;
}