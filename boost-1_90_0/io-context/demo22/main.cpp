#include <iostream>
#include <sstream>
#include <string>

#include <boost/asio.hpp>

void defer_task(boost::asio::io_context& context, const std::string& text) {
     std::ostringstream os;
     os << text;
     std::string temp = os.str();

     boost::asio::post(context, [&temp]{
        std::cout << temp << std::endl;
     });
}

int main() {
    boost::asio::io_context context;

    defer_task(context, "test");

    context.run();
    
    return 0;
}