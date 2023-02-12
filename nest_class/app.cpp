#include <iostream>

/// Nested classes are like other enclosing class members, so nested classes can
/// access private members of enclosing class, but rules have to be followed.
/// Nested classes do not have 'this' pointer of the enclosing class, so nested
/// classes can not access non-static members of the enclosing class, except
/// that during construction the 'this' pointer is passed to the nested class.

/// Different compiler might have variant treatment about how the nested classes
/// access memebers of the enclosing class. For example, gnu g++ allow
/// static-members AND unevaluated access of enclosing class, so one can use
/// sizeof(somemember), even though somemember is non-static. Clang does not
/// allow nested classes access any non-static members of enclosing class.
///
/// Enclosing class have no special right to access memebers of the nested
/// class.

/// Enclosing class acts like namespace; Nested class is for better
/// encapulasation


class A {
public:
  class B {
    friend void friendofB(A::B *);
    int b;

  public:
    B() { std::cout << "B is being created..\n"; }
    void print_g() { std::cout << "from nested b" << '\n'; }
    /// 也可以部分函数在类外定义
    void anotherfunc();
    void access_enclosing_memebers() {
      /// nested type 'B', 这里因为evaluate了a，所以不能访问
      /// Use of non-static data member 'a' of 'A' from
      //   std::cout << a << '\n';

      /// gnu g++: 这里没有evaluate a，sizeof(a)与A的instance无关，所以可以访问
      /// clang: use of non-static data member 'a' of 'A' from nested type 'B'
      /// different compiler treat this differently
      //   std::cout << sizeof(a) << '\n';

      /// 可以访问static成员，与instance无关
      std::cout << c << '\n';
    }

    void test_friendofB() { friendofB(this); }
  } b;

private:
  int a;
  const static char c = 'A';

  /// C仅有forward-declaration
  class C;
  /// incomplete type is not
  /// allowed.因为犯了在定义之前使用的错误，这里只是declaration，没有define
  //   C c;

  /// D既有declaration，又有define，但是没有realization；
  /// 注意forward-declaration/definition/realization三个概念的不同
  class D {
    /// 这里必须是public，不然A的default constructor将被删除
  public:
    D();
    void func_D() { std::cout << "Funtion from D\n"; }
  };

  /// 因为D已经在A中有定义（虽然没有具体实现），此处可以声明d对象;因为此处d的内存大小是已知的
  D d;

public:
  /// D虽然是私有变量，但是通过typedef一个public的D_, D_可以被外部使用
  typedef D D_;

  void print_f() {
    b.print_g();

    /// enclosing类不能访问内嵌类的私有成员：'b' is a private member of 'A::B'
    // b.b = 0;

    b.access_enclosing_memebers();
    b.test_friendofB();
    std::cout << "func\n";
  }
};

class A::C {};

/// nested class 可以在外部实现
void A::B::anotherfunc() {}

/// 定义
A::D::D() {}

/// 内嵌类跟普通成员一样，也有访问权限，private的内嵌类无法在外部使用：'C' is a
/// private member of 'A'
// void friendofC(A::C*para){}

/// 内嵌类的friend可以访问内嵌类的私有成员
void friendofB(A::B *ptr) {
  A a;

  /// 内嵌类的friend不是enclosing类的friend，不能访问enclosing类私有成员：'a' is
  /// a private member of 'A'
  //   a.a = 0;
  std::cout << "comming from friendofB:" << ptr->b << '\n';
}

int main() {

  A a;
  a.print_f();

  /// 'C' is a private member of 'A'
  //   A::C c;

  // OK, D_ is public
  A::D_ d;
  d.func_D();

  /// nested class is just like normal classes, enclosing class acts like
  /// namespace
  A::B b;
  b.access_enclosing_memebers();
  b.print_g();
}