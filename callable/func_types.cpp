#include <iostream>
#include <type_traits>

// 类型名称：void(int*);函数名称：func1;signature:void(int*)
void func1(int *);

// 类型名称：type_func; signature: void(int*)
typedef void type_func(int *);

// 类型名称为void(int*)的函数
type_func func2;

// 函数定义
void func1(int *) {}
void func2(int *) {}

// 指向类型名称为void(int*)的指针
void (*func_ptr1)(int *) = func1;
type_func *func_ptr2 = func2;

// 函数类型不能声明变量，也不能赋值
// Illegal initializer (only variables can be
// initialized)clang(illegal_initializer) type_func func3 = func1;

void compare_func(type_func func1, type_func func2) {
  if (func1 == func2) {
    std::cout << "func1 is equal to func2" << std::endl;
  } else {
    std::cout << std::boolalpha << std::is_pointer<decltype(func1)>::value
              << std::endl;
    std::cout << std::boolalpha << std::is_pointer<decltype(func2)>::value
              << std::endl;
    std::cout << "func1 is not equal to func2" << std::endl;
  }
}

// 错误，不可以直接返回函数类型
// type_func ret_func1() { return func1; }
// 正确，可以返回函数指针类型
type_func *ret_func2() { return func2; }

int main() { compare_func(func1, func2); }