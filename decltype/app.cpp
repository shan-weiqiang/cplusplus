#include <functional>
#include <iostream>
#include <vector>

struct T
{
  int *a;
};

int *func(int &data)
{
  return &data;
}

int &&rva(int &data)
{
  return static_cast<int &&>(data);
}

void foo()
{
  int x = 10;
  int *y = nullptr;
  int &&prvalue1 = 34;
  int &&prvalue2 = 34;
  int ar[] = { 1, 2, 3 };
  T t;

  /// 1) If the argument is an unparenthesized id-expression or an
  /// unparenthesized class member access expression, then decltype yields the
  /// type of the entity named by this expression.
  decltype(x) a = 1;
  decltype(t.a) p = &a;
  decltype(prvalue1) dpravlue = 98;
  decltype(rva(x)) q = 89;

  /// For any other expressions:
  // a) if the value category of expression is xvalue, then decltype yields T&&
  decltype(rva(prvalue1)) xvalue = 23;
  decltype(std::move(x)) super = 45;

  /// b.1) if the value category of expression is lvalue, then decltype yields
  /// T&;
  decltype(ar[0]) d = a;
  decltype(*y) c = a;
  decltype(*(t.a)) b = a;
  /// b.2) If id-expression or class member access expression is parenthesized,
  /// it's considered as lvalue
  decltype((a)) b1 = a;

  /// c) if the value category of expression is prvalue, then decltype yields T.
  decltype(3) e = 4;
  decltype(prvalue1 + prvalue2) m = 34;
}

int main()
{
  foo();
}