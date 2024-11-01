
#include <iostream>

struct Dummy {
    Dummy() {
        std::cout << "Dummy constructor called" << std::endl;
    }
    void f();
};

void Dummy::f() {}

Dummy d;
