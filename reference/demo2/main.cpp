#include <iostream>

class MyObject {
public:
    MyObject(int v): v_(v) {
        std::cout << "Create MyObject -- v_=" << v_ << std::endl;
    }

    MyObject(const MyObject& other): v_(other.v_) {
        std::cout << "Create MyObject --  Copy v_=" << v_ << std::endl;
    }

    MyObject(MyObject&& other): v_(other.v_) {
        std::cout << "Create MyObject --  Move v_=" << v_ << std::endl;
    }

    ~MyObject() {
        std::cout << "Destroy MyObject -- v_=" << v_ << std::endl;
    }

    // 这两个方法在例子中不需要
    // MyObject& operator=(const MyObject& other);
    // MyObject& operator=(MyObject&& other);

    friend MyObject operator+(const MyObject& other1, const MyObject& other2) {
        return MyObject(other1.v_ + other2.v_);
    }

private:
    int v_;
};

int main() {
    MyObject o1(1);
    MyObject o2(2);

    {
        std::cout << "\n===== Step 1 -- begin =====" << std::endl;

        o1 + o2;

        std::cout << "===== Step 1 -- end   =====" << std::endl;
    }

    {
        std::cout << "\n===== Step 2 -- begin =====" << std::endl;

        MyObject MyObject = o1 + o2;

        std::cout << "===== Step 2 -- end   =====" << std::endl;
    }

    {
        std::cout << "\n===== Step 3 -- begin =====" << std::endl;

        MyObject&& MyObject = o1 + o2;

        std::cout << "===== Step 3 -- end   =====" << std::endl;
    }

    {
        std::cout << "\n===== Step 4 -- begin =====" << std::endl;

        // 下面，这句会编译失败
        // MyObject& MyObject = o1 + o2;

        std::cout << "===== Step 4 -- end   =====" << std::endl;
    }

    return 0;
}