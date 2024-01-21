#include <functional>
#include <iostream>
#include <memory>
#include <type_traits>

class SampleClass {
public:
  SampleClass(const std::string &spc) : spec{spc} {}
  std::string spec;

  void print();
};

void SampleClass::print() { std::cout << this->spec << std::endl; }

int main() {

  SampleClass myclass{"hello"};

  // 指向成员函数的类型,注意括号中包含类的名称
  typedef void (SampleClass::*mem_ptr)();

  mem_ptr ptr = &SampleClass::print;
  // 调用print
  (myclass.*ptr)();

  // C++17以上的编译器，可以使用如下
  std::invoke(ptr, &myclass);
}
