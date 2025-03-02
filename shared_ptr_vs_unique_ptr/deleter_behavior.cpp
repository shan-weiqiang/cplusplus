#include <memory>
#include <iostream>

struct A {

    ~A() {
        std::cout << "A destructor\n";
    }
};

struct B : public A {

    ~B() {
        std::cout << "B destructor\n";
    }
};

struct C {

    virtual ~C() {
        std::cout << "C destructor\n";
    }
};

struct D : public C {

    ~D() override {
        std::cout << "D destructor\n";
    }
};

int main() {
    {
        std::cout << "When desctructor is non-virtual, std::shared_ptr will call correct destructor\n";
        std::shared_ptr<A> a = std::make_shared<B>();
    }
    std::cout << "----" << std::endl;
    {
        std::cout << "When destructor is virtual, std::shared_ptr will call correct destructor\n";
        std::shared_ptr<C> b = std::make_shared<D>();
    }
    std::cout << "----" << std::endl;
    {
        std::cout << "When destructor is non-virtual, std::unique_ptr will NOT call correct destructor\n";
        /// This is undefined behavior, since B is destructed by A's destructor
        std::unique_ptr<A> c = std::make_unique<B>();
    }
    std::cout << "----" << std::endl;
    {
        std::cout << "When destructor is virtual, std::unique_ptr will call correct destructor\n";
        /// Since destructor is virtual, D's destructor will be called
        std::unique_ptr<C> d = std::make_unique<D>();
    }
}
