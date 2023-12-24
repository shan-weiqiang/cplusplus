#include <functional>
#include <iostream>
#include <type_traits>

// std::function可以存储函数指针或者实现了()操作符的任意对象(function object):
// 1. 全局函数指针/类成员函数指针
// 2. std::bind的返回对象：function object
// 3. lamda表达式：function object
// 4. 任意实现了()操作符的类型
// 上面第2,3是第4的特殊情况，由编译器自动产生。
// std::function内部存储一组指针，如果其包裹的对象可以存下，例如函数指针，则不会在栈上分配内存；
// 如果包裹的对象无法在这组指针中存储，则会在栈上分配内存并存储该对象; std::function 支持拷贝构造
// 和移动构造，所以要求存储的对象必须是
// CopyConstructible的（详见注1），所以当std::function被move的时候，其栈上的对象被move

// 注1： copy constructor可以替代move constructor当参数为const T&:
// If both copy and move constructors are provided and no other constructors are viable, overload
// resolution selects the move constructor if the argument is an rvalue of the same type (an xvalue
// such as the result of std::move or a prvalue such as a nameless temporary (until C++17)), and
// selects the copy constructor if the argument is an lvalue (named object or a function/operator
// returning lvalue reference). If only the copy constructor is provided, all argument categories
// select it (as long as it takes a reference to const, since rvalues can bind to const references),
// which makes copying the fallback for moving, when moving is unavailable.
// A constructor is called a move constructor when it takes an rvalue reference as a parameter. It
// is not obligated to move anything, the class is not required to have a resource to be moved and a
// move constructor may not be able to move a resource as in the allowable (but maybe not sensible)
// case where the parameter is a const rvalue reference (const T&&).
std::function<void()> a;

struct A
{
	A() = default;
	~A() noexcept = default;
	A(const A&)
	{
		std::cout << "A::copy construct\n";
	}
	A(A&&) noexcept
	{
		std::cout << "A::move construct\n";
	}
	A& operator=(const A&) = default;
	A& operator=(A&&) noexcept = default;

	double c = 23;
};

template <typename T>
void traits()
{
	std::cout << std::boolalpha << "trivially_copy_constructible? "
			  << std::is_trivially_copy_constructible<T>::value
			  << "\ntrivially_move_constructible? "
			  << std::is_trivially_move_constructible<T>::value << '\n';
}

std::function<void()> func()
{
	A a;
	return [&a]() { std::cout << "value of c:" << a.c << std::endl; };
}

int main()
{
	A a;

	{
		// 闭包的移动和拷贝：
		// 1.
		// 如果闭包以拷贝的方式捕获了周边变量，则无论是闭包被移动还是被赋值，该捕获的变量都是拷贝
		// (很有可能闭包类型并没有实现移动构造，而是只有拷贝构造)
		// 2. 如果闭包以引用的方式捕获了周边变量，则闭包本身是trivially_copy_constructible
		// 和trivially_move_constructible
		// 3. 因为闭包本身就是一个局部创建的对象，所以闭包执行的时候要保证其捕获的变量还在声明周期内

		func()();

		auto f = func();
		std::cout << "now call some function to corrupt stack memory" << std::endl;
		// c的值已经被覆盖，undefined behavior
		f();

		std::cout << "capture by value\n";
		const auto closure = [a = std::move(a)] {}; // A::move construct
		traits<decltype(closure)>(); // not trivially_copy_constructible, not
			// trivially_move_constructible

		// copy constructor of the closure copies the object captured by value
		const auto copy_constructed = closure; // A::copy construct

		// move constructor of the closure copies (does not move) the object
		// captured by value
		const auto move_constructed = std::move(closure); // A::copy construct
	}

	{
		std::cout << "\ncapture by reference\n";
		const auto closure = [&a] {}; // capture by reference
		traits<decltype(closure)>(); // trivially_copy_constructible,
			// trivially_move_constructible
	}
}