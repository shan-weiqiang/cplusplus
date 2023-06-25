// The basis of C/C++ exception handling is nonlocal jumps, see
// Computer.Systems.A.Programmers.Perspective.3rd.Global.Edition.2015.7-csapp.pdf, chapter 8.6 for detailed information.
//
// The basic idea is that execution context is saved and when condition is met, goes directly to saved context without too
// many returns from nested function calls. It works very much like coroutine.
//
// Two function is used to achieve this:
//
// - `setjmp` used to save context
// - `longjmp` used to go to some saved context
//
// We can think of a `catch` clause inside a `try` statement as being akin to a `setjmp` function. Similarly, a `throw`
// statement is similar to a `longjmp` function.
#include <iostream>
#include <stdexcept>

struct A
{
  int n;

  A(int n = 0) : n(n)
  {
    std::cout << "A(" << n << ") constructed successfully\n";
  }
  ~A()
  {
    std::cout << "A(" << n << ") destroyed\n";
  }
};

int foo()
{
  throw std::runtime_error("error");
}

struct B
{
  A a1, a2, a3;

  B()
  try : a1(1), a2(foo()), a3(3)
  {
    std::cout << "B constructed successfully\n";
  }
  catch (...)
  {
    std::cout << "B::B() exiting with exception\n";
  }

  ~B()
  {
    std::cout << "B destroyed\n";
  }
};

struct C : A, B
{
  C()
  try
  {
    std::cout << "C::C() completed successfully\n";
  }
  catch (...)
  {
    std::cout << "C::C() exiting with exception\n";
  }

  ~C()
  {
    std::cout << "C destroyed\n";
  }
};

int main()
try
{
  // creates the A base subobject
  // creates the a1 member of B
  // fails to create the a2 member of B
  // unwinding destroys the a1 member of B
  // unwinding destroys the A base subobject
  C c;
}
catch (const std::exception& e)
{
  std::cout << "main() failed to create C with: " << e.what();
}
