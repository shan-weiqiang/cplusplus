#include <iostream>
#include <type_traits>
//    ______ ______
//   /      X      \
//  /      / \      \
// |   l  | x |  pr  |
//  \      \ /      /
//   \______X______/
//       gl    r
// Above diagram describes the general relationship between lvalue(l), xvalue(x), prvalue(pr), glvalue(gl), rvalue(r)
//
// Each C++ expression (an operator with its operands, a literal, a variable name, etc.) is characterized by two independent
// properties: a type and a value category. Each expression has some non-reference type, and each expression belongs to
// exactly one of the three primary value categories: prvalue, xvalue, and lvalue.
//
// We use expression to declare variables, only variables(has name) is related to reference. Expressions has nothing to do
// with reference yet. Reference is alias for existing object, and do not have object(memeory occupation) itself, which means
// reference must have a name(reference variable name), otherwise it can not exit, since it's alias. A name and a binding
// object are all reference have and must have.
//
// So for expression, let's say it has value type and value category; For variables, which is delcared using expression and
// must have names, let's say it might be of value type or reference type, which consists of lvalue reference and rvalue
// reference.
//
// Named variable is also one kind of expression, it has lvalue value type and non-reference type; What is special about
// named variables is that it is always lvalue and except non-reference types, it might be reference to existing object. In
// this case, the non-reference type of the existing object is the type of the variable and the variable itself is also a
// reference. It is extremely confusing, what must be remembered is that value category has no relationship with reference
// whatsoever. A variable can be lvalue and reference to lvalue at the same time, which is called lvalue reference; A
// variable can also be lvalue and reference to rvalue at the same time, which makes the variable rvalue reference. When we
// say a variable is lvalue or rvalue reference, we mean the expression that initialize this variable is lvalue or rvalue,
// for the variable itself, it's always lvalue. Considering all this, a named variable expression has three properties:
// 1. non-reference type: the type of the existing object, whether the variable directly points to or the variable refers to
// 2. reference type: if the variable is a reference, this is either lvalue reference or rvalue reference
// 3. value category: always lvalue category
//
// What decltype really does
//
// Inspects the declared type of an entity or the type and value category of an expression.
// This implies two funtionality of decltype:
//
// 1. When used as decltype ( entity ), where entity is unparenthesized id-expression or class memeber expression, it yields
// the type of entity
// 2. When used as decltype ( expression ), where expression is any other expression, it 	inspects the expression's
// value type(eg,T) and value category and yields following types accordingly:
//
// a. if value category of expression is xvalue, it yields T&&
// b. if value category of expression is lvalue, it yields T&
// c. if value category of expression is rvalue, it yields T
// Note: if variable id-expression or class memeber access expression is parenthesized, it is treated as ordinary lvalue
// expression(which is reasonable, because named variables are always lvalue expressions)
//
// How to use decltype to defer the value category of expression
//
// First approach, we can check whether the yield type of decltype(expression) is lvalue or rvalue reference, if rvalue
// reference, the expression is xvalue; if lvalue reference, the expression is lvalue; otherwise, the expression is prvalue

template<typename T>
constexpr bool is_lvalue_helper = std::is_lvalue_reference<T>::value;

template<typename T>
constexpr bool is_xvalue_helper = std::is_rvalue_reference<T>::value;

template<typename T>
constexpr bool is_prvalue_helper = !(is_lvalue_helper<T> || is_xvalue_helper<T>);

// Another way is to use template

template<typename T>
struct value_category
{
  // Or can be an integral or enum value
  static constexpr auto value = "prvalue";
};

template<typename T>
struct value_category<T&>
{
  static constexpr auto value = "lvalue";
};

template<typename T>
struct value_category<T&&>
{
  static constexpr auto value = "xvalue";
};

// Double parens for ensuring we inspect an expression,
// not an entity
#define VALUE_CATEGORY(expr) value_category<decltype((expr))>::value
