#include <iostream>
#include <type_traits>

typedef bool (*some)(int);

template<typename Fn>
void func(Fn func)
{
  std::cout << func(6) << std::endl;
}

int main()
{
  int a{ 1 };

  // ok, closure without any captures can be implicitly converted to function
  // pointer
  some s = [](int b) -> bool
  {
    std::cout << b << std::endl;
    return true;
  };

  // nok, the closure has capture
  //   some t = [&a](int b) -> bool
  //   {
  //     std::cout << b << std::endl;
  //     return true;
  //   };

  // both capture and no-capture closures can be used as template parameters
  func([](int b) -> bool { return false; });
  func(
      [&a](int b) -> bool
      {
        std::cout << a << std::endl;
        return true;
      });
}