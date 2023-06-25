/// 1. static memebers(including static method) are shared among all the subclasses
/// 2. if subclass define member with the same name, this memeber is different with the base class, they are in different
/// namesapce, they are Base::m, Derived:Base::m
/// 3. inside every constructor of the derived class, base class's constructor is called
#include <iostream>

class SomeClass
{
 public:
  SomeClass()
  {
    total++;
  }
  static int total;
};

int SomeClass::total = 0;

class SomeDerivedClass : public SomeClass
{
 public:
  /// ! inside derived constructor, base constructor will be called again !!
  SomeDerivedClass()
  {
    total++;
  }
};

class AnotherDerivedClass : public SomeClass
{
 public:
  AnotherDerivedClass()
  {
    /// This is AnotherDerivedClass::total. But under the hood SomeClass() will be called and
    /// AnotherDerivedClass::SomeClass::total++ also called
    total++;
  }
  static int total;
};
int AnotherDerivedClass::total = 0;

int main()
{
  SomeClass A;
  SomeClass B;
  SomeDerivedClass C;
  AnotherDerivedClass D;
  std::cout << "SomeClass.total=" << SomeClass::total << std::endl;
  std::cout << "SomeDerivedClass.total=" << SomeDerivedClass::total << std::endl;
  std::cout << "AnotherDerivedClass.total=" << AnotherDerivedClass::total << std::endl;
  std::cout << "AnotherDerivedClass::SomeClass.total=" << AnotherDerivedClass::SomeClass::total << std::endl;

  return 0;
}
// output:
// SomeClass.total=5
// SomeDerivedClass.total=5
// AnotherDerivedClass.total=1
// AnotherDerivedClass::SomeClass.total=5
