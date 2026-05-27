
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#endif 

#include "boost/asio.hpp"
#include "boost/nowide/iostream.hpp"
#include "boost/process.hpp"

int main() {
    auto ping_filename = boost::process::environment::find_executable("ping");

    if (ping_filename.empty()) {
        boost::nowide::cout << "没有找到程序: ping" << std::endl;
        return 1;
    }
    
    boost::asio::io_context context;
    boost::process::process proc(context.get_executor(),
        ping_filename,
        {
#ifdef _WIN32
            "-t", 
#endif
            "baidu.com"
        }
    );

    // 如果不调用 proc.wait()
    // proc 会析构，这会导致 terminate
    proc.wait();

    return 0;
}