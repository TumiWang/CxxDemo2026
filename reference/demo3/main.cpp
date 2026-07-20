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

    friend MyObject operator+(const MyObject& other1, const MyObject& other2) {
        return MyObject(other1.v_ + other2.v_);
    }

public:
    void print() {
        std::cout << "MyObject::print v_:" << v_ << std::endl;
    }

private:
    int v_;
};

void f1(MyObject obj) {
    obj.print();
}

void g1(MyObject obj) {
    f1(obj);
}

void f2(MyObject&& obj) {
    obj.print();
}

void g2(MyObject&& obj) {
    f2(std::move(obj));
}

void f3(MyObject& obj) {
    obj.print();
}

void g3(MyObject& obj) {
    f3(obj);
}

int main() {
    MyObject o1(1);
    MyObject o2(2);

    std::cout << "===== main   -- begin =====" << std::endl;

    {
        std::cout << "\n===== Step 1 -- begin =====" << std::endl;

        g1(o1 + o2);

        std::cout << "===== Step 1 -- end   =====" << std::endl;
    }

    {
        std::cout << "\n===== Step 2 -- begin =====" << std::endl;

        g2(o1 + o2);

        std::cout << "===== Step 2 -- end   =====" << std::endl;
    }

    {
        std::cout << "\n===== Step 3 -- begin =====" << std::endl;

        // g3(o1 + o2); 会编译失败
        // 只能这么用
        MyObject o3 = o1 + o2;
        g3(o3);

        std::cout << "===== Step 3 -- end   =====" << std::endl;
    }

    std::cout << "\n===== main   -- end   =====" << std::endl;

    return 0;
}