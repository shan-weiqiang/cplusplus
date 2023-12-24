#include <algorithm>
#include <functional>
#include <iostream>
#include <typeinfo>
#include <vector>

#include "value_catgory.h"
#define SHOW_VALUE_CATEGORY(expr)                                                                  \
	std::cout << std::boolalpha;                                                                   \
	std::cout << "Is " << #expr << " lvalue:" << is_lvalue_helper<decltype((expr))> << std::endl;  \
	std::cout << "Is " << #expr << " xvalue:" << is_xvalue_helper<decltype((expr))> << std::endl;  \
	std::cout << "Is " << #expr                                                                    \
			  << " prvalue:" << is_prvalue_helper<decltype((expr))> << std::endl;                  \
	std::cout << #expr << " is:" << VALUE_CATEGORY(expr) << std::endl;                             \
	std::cout << "<<<<<<" << std::endl;

struct T
{
	int* a;
};

int* func(int& data)
{
	return &data;
}

int&& rva(int& data)
{
	return static_cast<int&&>(data);
}

void foo()
{
	int x = 10;
	int* y = nullptr;
	int&& prvalue1 = 34;
	int&& prvalue2 = 34;
	int ar[] = {1, 2, 3};
	T t;

	int status;

	/// 1) If the argument is an unparenthesized id-expression or an
	/// unparenthesized class member access expression, then decltype yields the
	/// type of the entity named by this expression.
	std::cout << "<<<<<<<<<" << std::endl;
	// int
	decltype(x) a = 1;
	/// abi::__cxa_demangle print the value type
	/// std::is... print whether it's reference
	std::cout << std::is_rvalue_reference<decltype(x)>::value << std::endl;
	std::cout << std::is_lvalue_reference<decltype(x)>::value << std::endl;
	std::cout << "<<<<<<<<<" << std::endl;
	// int*
	decltype(t.a) p = &a;
	std::cout << std::is_rvalue_reference<decltype(t.a)>::value << std::endl;
	std::cout << std::is_lvalue_reference<decltype(t.a)>::value << std::endl;
	std::cout << "<<<<<<<<<" << std::endl;
	// int&&
	decltype(prvalue1) dpravlue = 98;
	// This will not show reference infomation, only value type
	std::cout << std::is_rvalue_reference<decltype(prvalue1)>::value << std::endl;
	std::cout << std::is_lvalue_reference<decltype(prvalue1)>::value << std::endl;
	std::cout << "<<<<<<<<<" << std::endl;

	// error: non-const lvalue reference to type 'int' cannot bind to a temporary of type 'int'
	// the parenthesize treat prvalue1 as lvalue(all named variables are by themselves lvalue)
	// this is int&
	// decltype((prvalue1)) dlvalue = 90;

	/// For any other expressions:
	// a) if the value category of expression is xvalue, then decltype yields T&&
	// following are all int&&
	decltype(rva(prvalue1)) xvalue = 23;
	decltype(std::move(x)) super = 45;
	decltype(rva(x)) q = 89;
	std::cout << std::is_rvalue_reference<decltype(rva(x))>::value << std::endl;
	std::cout << std::is_lvalue_reference<decltype(rva(x))>::value << std::endl;

	/// b.1) if the value category of expression is lvalue, then decltype yields
	/// T&;
	/// int&
	decltype(ar[0]) d = a;
	decltype(*y) c = a;
	decltype(*(t.a)) b = a;
	/// b.2) If id-expression or class member access expression is parenthesized,
	/// it's considered as lvalue
	/// int&
	decltype((a)) b1 = a;

	/// c) if the value category of expression is prvalue, then decltype yields T.
	/// int
	decltype(3) e = 4;
	decltype(prvalue1 + prvalue2) m = 34;
}

int main()
{
	foo();
	// named variables are always lvalue by itself!!
	// the object it points to decide whether it's lvalue reference, rvalue reference or not a reference.
	//
	int x = 10;
	int& y = x;
	int&& a = 10;
	std::cout << "a is lvalue, address:" << &a << std::endl;
	int c = 3;
	// std::cout << "std::move(c) is rvalue, can not take address:" << &(std::move(c)) << std::endl;
	int m = std::move(c);
	std::cout << "m is lvalue, address: " << &m << std::endl;
	// This is called array lvalue, which means an array which is lvalue
	// The element acess using subscript is also lvalue
	int arr[] = {1, 2, 3};
	SHOW_VALUE_CATEGORY(x);
	SHOW_VALUE_CATEGORY(std::move(x));
	SHOW_VALUE_CATEGORY(y);
	SHOW_VALUE_CATEGORY(a);
	SHOW_VALUE_CATEGORY(arr[0]);
	// arr is moved and becomes array rvalue, the element access becomes xvalue
	SHOW_VALUE_CATEGORY(std::move(arr)[0]);
	// arr_m is named, it's lvalue and arr_m[0] is also lvalue
	// A named variable expression is always lvalue value category; the type of the variable expression is the type of the
	// expression that used to initialize this variable; if the object that the expression denotes is already existing, the
	// variable is also reference type, lvalue reference or rvalue reference according to the value category of the expression
	// that initialzie this variable.
	auto arr_m = std::move(arr);
	SHOW_VALUE_CATEGORY(arr_m);
	SHOW_VALUE_CATEGORY(arr_m[0]);
	SHOW_VALUE_CATEGORY(1);
	/// IMPORTANT: without the parenthesize, such as decltype(y), this yields the infomation that y points to, in this example
	/// it's a lvalue reference, which means:
	///   1. the existing object that y points to is lvalue
	///   2. y itself is reference
	///
	///   BUT: the value category of expression 'y' itself is lvalue
	///   This applies to variable a, which is rvalue reference, which means the exisitng object that a points to is rvalue and
	///   a is a reference but the expression 'a' itself is lvalue, just like any other named variables
	///
	///   if one want to get the value category of the expression itself, should use parenthesize like decltype((a)) or
	///   decltype((y))
	///
	///   to verify this point, following statement is ok:
	///   int &b = a;
	///   at first glance it is weired because a is rvalue reference, how can lvalue reference b points to a?
	///   the key points here is that a is actually lvalue, it's a reference and it points to a rvalue, so it's called rvalue
	///   reference
	///   IMPORTANT: in C++, a special case is that const lvalue reference can be assigned with rvalue
	std::cout << std::is_rvalue_reference<decltype(y)>::value << std::endl;
	std::cout << std::is_lvalue_reference<decltype(y)>::value << std::endl;
	std::cout << std::is_rvalue_reference<decltype(a)>::value << std::endl;
	std::cout << std::is_lvalue_reference<decltype(a)>::value << std::endl;
}
