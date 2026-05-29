
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#endif 

#include "boost/nowide/iostream.hpp"
#include "boost/process.hpp"

int main()
{
    // 获取当前的进程ID
    auto process_id = boost::process::current_pid();

    // 获取指定进程的全路径文件名称
    auto process_filename = boost::process::ext::exe(process_id);
    boost::nowide::cout << "当前进程ID为 "
        << process_id
        << " 文件名称为 "
        << process_filename
        << std::endl;

    return 0;
}