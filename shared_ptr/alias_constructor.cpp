#include <memory>
#include <iostream>

struct A {
    A() {
        std::cout << "A constructor\n";
    }

    ~A() {
        std::cout << "A destructor\n";
    }
};

struct B {
    B() {
        std::cout << "B constructor\n";
    }

    ~B() {
        std::cout << "B destructor\n";
    }
};

struct C {
    C() {
        std::cout << "C constructor\n";
    }

    ~C() {
        std::cout << "C destructor\n";
    }
    A a;
    B b;
};

int main() {
    std::shared_ptr<A> a_ptr;
    {
        std::shared_ptr<C> ptr = std::make_shared<C>();
        // a_ptr shared ref count with ptr, but point to ptr.a
        a_ptr = std::shared_ptr<A>{ptr, &ptr->a};
    }
    // ptr is out of scope, but object it points to is not destructed yet,since a_ptr still holds a reference to ptr.a
    std::cout << "C's construct should not be destructed yet\n";
}
