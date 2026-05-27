
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#endif 

#include "boost/asio.hpp"
#include "boost/nowide/iostream.hpp"

// 之前使用 #include "boost/process.hpp"
// 通常是使用 boost::process::v2
// 下面是使用 boost::process::v1
#define BOOST_PROCESS_VERSION 1
#include "boost/process/v1.hpp"

int main() {
    auto ping_filename = boost::process::search_path("ping");

    if (ping_filename.empty()) {
        boost::nowide::cout << "没有找到程序: ping" << std::endl;
        return 1;
    }
    
    boost::process::child proc(
        ping_filename,
#ifdef _WIN32
        "-t",
#endif
        "baidu.com"
    );

    proc.wait();

    return 0;
}