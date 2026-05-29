
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#endif 

#include "boost/asio.hpp"
#include "boost/nowide/iostream.hpp"
#include "boost/process.hpp"

#include <string>
#include <vector>

// 主进程查询的环境变量
#define DEFAULT_ENV_KEY "TERM"

// 工作进程中 环境变量 DEFAULT_ENV_KEY 的值
// 注： 主进程中 这个环境变量是系统值(实际上是来源于主进程的启动环境)
#define DEFAULT_ENV_VALUE "This is a TEST"

// 打印进程信息 -- 作为其它输出信息的前缀
void PrintProcessInfo() {
    boost::nowide::cout << "当前进程[" << boost::process::current_pid() << "] ";
}

void PrintEnvValue(const std::string& key) {
    boost::system::error_code ec;
    auto v = boost::process::environment::get(key.c_str(), ec);

    PrintProcessInfo();
    if (ec) {
        boost::nowide::cout << "未找到环境变量 " << key << std::endl;
    } else {
        boost::nowide::cout << "环境变量 "
            << key
            << " ---- "
            << v.string()
            << std::endl;
    }
}

void DoMainProcess() {
    PrintEnvValue(DEFAULT_ENV_KEY);

    // 下面代码是为工作进程准备环境变量 my_env
    std::vector<boost::process::environment::key_value_pair> my_env;
    my_env.push_back(boost::process::environment::key_value_pair(DEFAULT_ENV_KEY, DEFAULT_ENV_VALUE));
    for (const auto& kv: boost::process::environment::current()) {
        if (kv.key().string() != DEFAULT_ENV_KEY)
            my_env.push_back(kv);
    }

    // 工作进程的全文件名
    auto process_filename = boost::process::ext::exe(boost::process::current_pid());

    // 启动工作进程前打印一些信息
    PrintProcessInfo();
    boost::nowide::cout << "即将启动工作进程" << std::endl;

    // 启动工作进程
    boost::asio::io_context context;
    boost::process::process proc(
        context,
        process_filename,
        { DEFAULT_ENV_KEY }, // 工作进程有一个启动参数
        boost::process::process_environment(my_env)
    );
    // 等待工作进程完成
    proc.wait();
    
    // 主进程中打印工作进程的信息
    PrintProcessInfo();
    boost::nowide::cout << "工作进程[" << proc.id() << "]已经退出" << std::endl;
}

// 该程序没有参数启动时，认为是主进程
// 主进程会启动自己作为子进程 -- 工作进程
// 工作进程有一个参数
int main(int argc, char* argv[])
{
    // 根据启动参数判断是主进程还是其它的辅助进程
    // 当然，这里只有一种辅助进程
    if (argc == 1)
    {
        // argv[0] 通常当前进程的文件名称
        // 但根据运行方式和运行环境的差别
        // 它可能不是全路径的文件名 -- 所以不用这种方法

        // 当参数只有一个时, 认为是主进程
        DoMainProcess();
    }
    else
    {
        // 此时认为是工作进程
        // 这时 应该满足 argc >= 2
        // 顾, 认为 argv[1] 是要查询的环境变量
        PrintEnvValue(argv[1]);
    }

    return 0;
}