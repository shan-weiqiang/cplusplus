#include <functional>
#include <iostream>

class FunctorClass {
  public:
    int a{1};

    void operator()() { std::cout << a << std::endl; }
};

int main() {
    FunctorClass f;
    f();

    // since C++17
    std::invoke(f);
}