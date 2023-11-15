#include <iostream>
#include <memory>

//  成员非静态方法重载时要考虑const, non-const, ref-qualified,
//  non-ref-qualified。 类本身作为第一个参数（implicit object parameter),
//  以下简称IOP. 注意： constructor、destructor、conversion function没有
//  implicit object parameter,所以不能使用const或者ref-qualifier.
//
//  带ref-qualifer与不带ref-qualifer的同名方法不能共存，带ref-qualifier的方法
//  与不带ref-qualifier的方法不能相互重载
//  
//  默认情况下，所有implicit object member function(https://en.cppreference.com/w/cpp/language/member_functions)
//  都需要考虑const-qualifier|non-const-qualifier, non-ref-qualifer|(rvalue-ref-qualifier, lvalue-ref-qualifier), 
//  共计：2x1+2x2=6种情况：
//
//  A. non-const-qualifier：
//     #1. 不带ref-qualifer的方法：
//         - IOP是non-const传入
//         - IOP可以是左值，也可以是右值
//     - 带ref-qualifer的方法，则：
//       #2. 带&，则IOP只能以左值传入
//       #3. 带&&，则IOP只能以右值传入
//
//  B. const-qualifier：
//     #4. 不带ref-qualifer:
//         - IOP是带const传入
//         - IOP可以是常量左值，也可以是常量右值
//         - IOP可以是非常量左值，也可以是非常量右值
//     - 带ref-qualifier的方法，则：
//       #5. 带&，则IOP以常量左值、或者非常量左值传入
//       #6. 带&&，则IOP以常量右值、或者非常量右值传入

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
