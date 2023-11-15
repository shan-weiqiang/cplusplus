#include <iostream>
#include <memory>

//  成员非静态方法重载时要考虑const, non-const, ref-qualified,
//  non-ref-qualified。 累本身作为第一个参数（implicit object parameter),
//  以下简称IOP. 注意： constructor、destructor、conversion function没有
//  implicit object parameter,所以不能使用const或者ref-qualifier.
//
//  带ref-qualifer与不带ref-qualifer的同名方法不能共存，带ref-qualifier的方法
//  与不带ref-qualifier的方法不能相互重载
//
//  没有const的情况：
//  不带ref-qualifer的方法：
//     - 只会匹配non-const的IOP
//     - 在non-const的前提下，同时支持左值和右值的IOP的传入
//  带ref-qualifer的方法，则：
//     - 带&的方法只能允许左值对象传入
//     - 带&&的方法只能允许右值对象传入
//
//  针对有const的情况：
// An implicit object member function with a cv-qualifier sequence
// of class X is treated as follows:
// - no ref-qualifier: the implicit object parameter has type lvalue reference
// to cv-qualified X and is additionally allowed to bind rvalue implied object
// argument
// - lvalue ref-qualifier: the implicit object parameter has type lvalue
// reference to cv-qualified X
// - rvalue ref-qualifier: the implicit object parameter has type rvalue
// reference to cv-qualified X

class MemoryBlock {
public:
  // 与const qualifier 不同，ref-qualifer 不能与不含ref-qualifer的方法同时出现
  // MemoryBlock &operator=(MemoryBlock &&) {
  //   std::cout << "called" << std::endl;
  //   return *this;
  // }

  MemoryBlock &operator=(MemoryBlock &&) & {
    std::cout << "& called" << std::endl;
    return *this;
  }

  const MemoryBlock &operator=(MemoryBlock &&) const & {
    std::cout << "const & called" << std::endl;
    return *this;
  }

  MemoryBlock &operator=(MemoryBlock &&) && {
    std::cout << "&& called" << std::endl;
    return *this;
  }
  const MemoryBlock &operator=(MemoryBlock &&) const && {
    std::cout << "const && called" << std::endl;
    return *this;
  }
};

int main() {

  MemoryBlock m;
  m = MemoryBlock();
  const_cast<const MemoryBlock &>(m) = MemoryBlock();

  std::move(m) = MemoryBlock();
  const_cast<const MemoryBlock &&>(std::move(m)) = MemoryBlock();
}
