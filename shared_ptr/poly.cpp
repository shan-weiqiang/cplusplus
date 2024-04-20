#include <iostream>
#include <memory>

struct BaseType {
    BaseType() {
        std::cout << "BaseType constructor\n";
    }
    virtual ~BaseType() {
        std::cout << "BaseType destructor\n";
    }
};

struct DerivedType : public BaseType {
    DerivedType() {
        std::cout << "DerivedType constructor\n";
    }
    ~DerivedType() override {
        std::cout << "DerivedType destructor\n";
    }
};

int main() {
    // 尽管ptr是std::shared_ptr<BaseType>类型，但是因为control block指向的是DerivedType对象，所以DerivedType的析构函数会被调用
    std::shared_ptr<BaseType> ptr = std::make_shared<DerivedType>();
}