#include <functional>
#include <iostream>
#include <memory>
#include <random>

void f(int n1, int n2, int n3, const int &n4, int n5) {
  std::cout << n1 << ' ' << n2 << ' ' << n3 << ' ' << n4 << ' ' << n5 << '\n';
}

int main() {

  int n = 7;
  // 占位符表示的是f1被实际调用时，用户传入的参数位置
  auto f1 = std::bind(f, std::placeholders::_2, 42, std::placeholders::_1,
                      std::cref(n), n);

  n = 8;

  // 注意n的值的区别
  f1(1, 2); // 2,42,1,8,7

  auto f2 = [](int a, int b) { std::cout << a << b << std::endl; };
  // bind一个lamda；lamda表达式也是一个函数对象
  auto f3 = std::bind(f2, 1, std::placeholders::_1);

  f3(2);
}