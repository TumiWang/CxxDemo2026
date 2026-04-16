#include <atomic>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/asio.hpp>

int CreateID() {
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

    {
        A a;
        // 变量a 的不会方式改成值捕获
        // 值捕获就是通过拷贝构造创建了另一个(同名的)变量
        boost::asio::post(context, [a] {
            a.print();
        });
    }

    std::cout << "开始运行" << std::endl;

    context.run();
    
    std::cout << "运行完成" << std::endl;
    
    return 0;
}