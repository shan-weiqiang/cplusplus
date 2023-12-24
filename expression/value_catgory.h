#include <iostream>
#include <type_traits>


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
