#include <functional>
#include <iostream>

void f(int *) { std::cout << "func called" << std::endl; }

class Funtor {
public:
  Funtor(const std::string &name) : name{name} {}
  std::string name;
  std::string operator()() { return this->name; }
  std::string get_name() {
    std::cout << "get_name called" << std::endl;
    return this->name;
  }
  std::string get_name_2() {
    std::cout << "get_name_2 called" << std::endl;
    return this->name;
  }
};

typedef std::string (Funtor::*get_name)();

int main() {
  std::function<void(int *)> funtor = f;
  int a;
  funtor(&a);
  // 指向类成员函数的指针
  get_name get = &Funtor::get_name;
  Funtor func{"elela"};
  (func.*get)();

  // std::bind可以bind指向成员函数的Callable
  std::bind(get, &func)();

  // std::function 不能直接存储一个成员函数指针，需要通过std::bind传递
  // std::function<get_name> func_wrapper;
  std::function<std::string(void)> func_wrap =
      std::bind(&Funtor::get_name_2, &func);
  func_wrap();
}