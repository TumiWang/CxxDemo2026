#include <atomic>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/asio.hpp>

int CreateID() {
    // 这个是线程安全的
    // 但实际上, 这个例子中没有多
    static std::atomic<int> current_id(0);
    return ++current_id;
}

class A {
public:
    A(): id_(CreateID()) {
        std::cout << "默认构造 A -- " << id_ << std::endl;
    }

    A(const A& other) {
        id_ = CreateID();
        std::cout << "使用(" << other.id_ << ")拷贝构造 A -- " << id_ << std::endl;
    }

    A(A&& other) {
        id_ = CreateID();
        std::cout << "使用(" << other.id_ << ")移动构造 A -- " << id_ << std::endl;
    }

    ~A() {
        std::cout << "析构 A -- " << id_ << std::endl;
    }

    void print() const {
        std::cout << "这是A -- " << id_ << std::endl;
    }

private:
    int id_;
};

int main() {
    boost::asio::io_context context;

    // 这个是控制变量 a 的生命周期
    {
        A a;
        boost::asio::post(context, [&a] {
            a.print();
        });
    }

    std::cout << "开始运行" << std::endl;

    context.run();
    std::cout << "运行完成" << std::endl;
    
    return 0;
}