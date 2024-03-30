#include <functional>
#include <iostream>

class _Undefined_class;

// std::function能在栈上存储的最大大小
union _Nocopy_types {
    void *_M_object;
    const void *_M_const_object;
    void (*_M_function_pointer)();
    void (_Undefined_class::*_M_member_pointer)();
};

void func(int a, double b, double c) { std::cout << a << std::endl; }

int main() {
    std::cout << "Size of _Nocopy_types: " << sizeof(_Nocopy_types) << " bytes"
              << std::endl;

    int a = 3;
    double b = 3.14;
    double c = 3.14;
    auto bind_func = std::bind(func, a, b, c);
    auto lamda_func = [&]() { func(a, b, c); };
    std::cout << "size of bind_func:" << sizeof(bind_func) << std::endl;
    std::cout << "size of lamda_func:" << sizeof(lamda_func) << std::endl;
    return 0;
}