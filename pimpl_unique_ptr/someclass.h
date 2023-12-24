
#include <iostream>
#include <memory>

template<typename T>
class SomeTemplateClass
{
 public:
  void print_size();
};

class SomeClass
{
 public:
  void do_some_thing();
  ~SomeClass();
  SomeClass();

 private:
  class SomeClassImp;
  // if dtor or ctor are not defined after the definition SomeClassImp(compiler
  // generated ctor and dtor are at the definiton of the SomeClass, if user
  // defines ctor and dtor, compiler will compile these functions at their
  // difinition), there will be incomplete type error. The reason behind this
  // can be explained in three levels:
  //
  // 1. dtor and ctor compile time and code inside these special functions. The
  // compile time of dtor and ctor is when their definition appears in the
  // source file. If no user defined functions found, compiler will compile
  // these functions at declaration of the class. The content of the ctor and
  // dtor not only include the user written code block(or empty if compiler
  // generated), but also all the ctor and dtor of base classes and members of
  // the class. All above functions will be inserted into the ctor and dtor of
  // the current class, in a predefined order: for ctor: base class ctor in the
  // order of their appearances-->class member ctor in the order of their
  // apperances-->code block inside ctor of current class. The calling sequence
  // of the dtor is in the reverse order for the ctor. In summary, all relevent
  // dtors and ctors are compiled at the apperance of definition of ctor and
  // dtor of current class and are all inserted inside this definition.
  //
  // 2. ctors and dtors are related. When the compiler compiles the ctors, it
  // need to reference to the dtors because inside the ctor the compiler has to
  // know how to destruct members if exception occurs during construction
  //
  // 3. The instantiation of shared_ptr and unique_ptr functions, such as dtor
  // and ctor and other member function has different requirements for the
  // completeness of the type arguments. Some functions need complete type
  // infomation, but some others do not. Generally speaking, it should be a
  // complete type for the template parameter,but shared_ptr and unique_ptr can
  // accept incomplete data type for the use of design pattern such as pimpl.
  // Following article have detailed explaination of which function
  // instantiation need incomplete or complete type.
  // https://stackoverflow.com/questions/6012157/is-stdunique-ptrt-required-to-know-the-full-definition-of-t/6089065#6089065
  //
  // In summary, the ctor need definition of dtor for unique_ptr instance, dtor
  // need complete definition of the type parameter of template argument. For
  // this to work, the complier need to compile the parent class ctor and dtor
  // with the definiton of the template data type in the same translation unit.
  std::unique_ptr<SomeClassImp> ptr;
  SomeTemplateClass<SomeClassImp> size;

  // Following pattern compiles but might not work well, because inside the dtor
  // of SomeClass, will call delete ptr, but the ~SomeClassImp() funtion is not
  // called.
  //
  // SomeClassImp *ptr;
};
