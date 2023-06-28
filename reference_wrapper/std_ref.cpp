#include <functional>
#include <iostream>

/// std::ref and std::cref are helper functions used to return a reference wrappers

void f(int& n1, int& n2, const int& n3)
{
  std::cout << "In function: " << n1 << ' ' << n2 << ' ' << n3 << '\n';
  ++n1;  // increments the copy of n1 stored in the function object
  ++n2;  // increments the main()'s n2
         // ++n3; // compile error
}

int main()
{
  int n1 = 1, n2 = 2, n3 = 3;
  /// here n1 is fixed for bound_f, but n2 and n3 are reference to n2 and n3, amoung which n3 is const and can not be changed
  std::function<void()> bound_f = std::bind(f, n1, std::ref(n2), std::cref(n3));
  n1 = 10;
  n2 = 11;
  n3 = 12;
  std::cout << "Before function: " << n1 << ' ' << n2 << ' ' << n3 << '\n';
  bound_f();
  std::cout << "After function: " << n1 << ' ' << n2 << ' ' << n3 << '\n';
}
