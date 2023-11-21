#include <iostream>
#include <memory>

//  成员非静态方法重载时要考虑const, non-const, ref-qualified,
//  non-ref-qualified。 类本身作为第一个参数（implicit object parameter),
//  以下简称IOP. 注意： constructor、destructor、conversion function没有
//  implicit object parameter,所以不能使用const或者ref-qualifier.
//
//  带ref-qualifer与不带ref-qualifer的同名方法不能共存，带ref-qualifier的方法
//  与不带ref-qualifier的方法不能相互重载，这包括带const和不带const的所有方法。
//
//  默认情况下，所有implicit object member
//  function(https://en.cppreference.com/w/cpp/language/member_functions)
//  都需要考虑const-qualifier|non-const-qualifier,
//  non-ref-qualifer|(rvalue-ref-qualifier, lvalue-ref-qualifier),
//  共计：2x1+2x2=6种情况：
//
//  A. non-const-qualifier：
//     - IOP必须是non-const传入
//     #1. 不带ref-qualifer的方法：
//         - IOP可以是左值（引用），右值（引用）
//     - 带ref-qualifer的方法，则：
//       #2.带&，则IOP只能以左值（引用），右值引用（右值引用没有完美转发，看作左值）
//       #3. 带&&，则IOP只能以右值传入; 注意右值引用传入后被视为左值
//
//  B. const-qualifier：
//     #4. 不带ref-qualifer:
//         - IOP可以是常量左值（引用），非常量左值（引用），非常量右值（引用），也可以是常量右值引用
//           （常量右值引用没有完美转发，则被看作左值）
//     - 带ref-qualifier的方法，则：
//     #5.带&，则IOP以常量左值(引用）、非常量左值（引用）、常量右值引用（常量右值引用没有完美转发，
//          则被看作左值），非常量右值引用，非常量右值（重载优先级低于#6）
//     #6.带&&，则IOP以非常量右值传入；重载优先级高于#5；注意右值引用传入后被视为左值
//  重要：常量右值引用是個左值;
//  常量右值本身在C++中是无法表示的，没有对应的expression

class MemoryBlock
{
 public:
  /// can not overload by ref-qualifier
  void non_const_non_ref_cant_overload()
  {
    std::cout << __LINE__ << " Non const non ref method called" << std::endl;
  }
  /// Cannot overload a member function with ref-qualifier '&' with a member
  /// function without a ref-qualifierclang(ref_qualifier_overload)
  // void non_const_non_ref_cant_overload() &{
  //   std::cout << "Non const non ref method called" << std::endl;
  // }

  /// Cannot overload a member function with ref-qualifier '&&' with a member
  /// function without a ref-qualifierclang(ref_qualifier_overload
  // void non_const_non_ref_cant_overload() && {
  //   std::cout << "Non const non ref method called" << std::endl;
  // }
  void non_const_can_overload() &
  {
    std::cout << __LINE__ << " Non const lvalue ref method called" << std::endl;
  }
  void non_const_can_overload() &&
  {
    std::cout << __LINE__ << " Non const rvalue ref method called" << std::endl;
  }

  void const_cant_overload() const
  {
    std::cout << __LINE__ << " Const non ref method called" << std::endl;
  }
  /// Cannot overload a member function with ref-qualifier '&' with a member
  /// function without a ref-qualifierclang(ref_qualifier_overload)
  // void const_cant_overload() const& {
  //   std::cout << "Const non ref method called" << std::endl;
  // }

  /// Cannot overload a member function with ref-qualifier '&&' with a member
  /// function without a ref-qualifierclang(ref_qualifier_overload)
  // void const_cant_overload() const&& {
  //   std::cout << "Const non ref method called" << std::endl;
  // }

  void const_can_overload() const&
  {
    std::cout << __LINE__ << " Const lvalue ref method called" << std::endl;
  }
  void const_can_overload() const&&
  {
    std::cout << __LINE__ << " Const rvalue ref method called" << std::endl;
  }
};

int main()
{
  std::cout << "<<<< #1 <<<<" << std::endl;
  MemoryBlock m;
  /// #1: ok, non-const lvalue
  m.non_const_non_ref_cant_overload();
  /// #1: ok, non-const rvalue
  MemoryBlock().non_const_non_ref_cant_overload();
  MemoryBlock& mr = m;
  /// #1: ok, lvalue reference
  mr.non_const_non_ref_cant_overload();
  MemoryBlock&& m_rvalue_ref = MemoryBlock();
  // #1: ok, rvalue reference
  m_rvalue_ref.non_const_non_ref_cant_overload();
  const MemoryBlock cm;
  /// A: nok, only accept non const
  // cm.non_const_non_ref();
  // const MemoryBlock&& cmr=std::move(m);
  /// A: nok, only accept non const
  // cmr.non_const_non_ref();
  std::cout << "<<<< #2 <<<<" << std::endl;

  MemoryBlock n;
  /// #2: ok, non const lvalue, overload void non_const_can_overload() &, if
  /// only define void non_const_can_overload() &&, there will be overload
  /// failure
  n.non_const_can_overload();
  MemoryBlock& n_lvalue_ref = n;
  /// #2: ok, non const lvalue reference
  n_lvalue_ref.non_const_can_overload();

  /// #2: ok, non const rvalue reference
  MemoryBlock&& n_rvalue_ref = MemoryBlock();
  n_rvalue_ref.non_const_can_overload();

  std::cout << "<<<< #3 <<<<" << std::endl;

  /// #3: ok, non const rvalue, overload void non_const_can_overload() &&, if
  /// only define void non_const_can_overload() &, there will be overload
  /// failure
  MemoryBlock().non_const_can_overload();

  /// A: nok, only accetp non const
  /// cm.non_const_can_overload();
  std::cout << "<<<< #4 <<<<" << std::endl;

  /// #4: ok, non-const lvalue
  m.const_cant_overload();
  /// #4: ok, non-const lvalue reference
  mr.const_cant_overload();
  /// #4: ok, const lvalue
  cm.const_cant_overload();
  const MemoryBlock& cons_lvalue_ref = m;
  /// #4: ok, const lvalue reference
  cons_lvalue_ref.const_cant_overload();
  /// #4: ok, non-const rvalue
  MemoryBlock().const_cant_overload();
  MemoryBlock&& non_const_rvalue_ref = MemoryBlock();
  /// #4: ok, non const rvalue reference
  non_const_rvalue_ref.const_cant_overload();
  const MemoryBlock&& cmr = MemoryBlock();
  /// #4: ok, const rvalue reference is lvalue, except using std::forward
  cmr.const_cant_overload();
  std::cout << "<<<< #5 <<<<" << std::endl;

  /// #5: ok, const lvalue, overload void const_can_overload() const&, if only
  /// define void const_can_overload() const&&, there will be overload failure
  cm.const_can_overload();
  /// #5: ok, const lvalue reference
  cons_lvalue_ref.const_can_overload();
  /// #5: ok, non const lvalue reference
  n_lvalue_ref.const_can_overload();
  /// #5: ok, non const lvalue
  m.const_can_overload();
  /// #5: ok, const rvalue reference is lvalue, except using std::forward
  cmr.const_can_overload();
  /// #5: ok, non const rvalue reference
  non_const_rvalue_ref.const_can_overload();

  std::cout << "<<<< #6 <<<<" << std::endl;

  /// #6: ok, non const rvalue；overload void const_can_overload() const&&, if
  /// void const_can_overload() const&& is not defined, void
  /// const_can_overload() const& will be overloaded,since const lvalue
  /// reference can bind to rvalue
  MemoryBlock().const_can_overload();
}