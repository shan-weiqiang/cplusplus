#include <iostream>
#include <vector>

namespace one

{
struct mystruct
{
	int b;
};

///@brief nonmember oprator must have class or enum as parameters
/// compiler use namespace of class and enum to overload operator
bool operator<<(mystruct& a, mystruct& b)
{
	if(a.b < b.b)
	{
		std::cout << "namespace one: a < b" << std::endl;
	}
}
} // namespace one
namespace two
{
struct mystruct
{
	int b;
};
bool operator<<(mystruct& a, mystruct& b)
{
	std::cout << "namespace two: a < b" << std::endl;
}
void f()
{
	std::cout << "f of namespace two\n";
};
} // namespace two

bool operator>>(two::mystruct& a, two::mystruct& b)
{
	std::cout << "namespace two: a > b" << std::endl;
}

/// Folling code will cause: Use of overloaded operator '<<' is ambiguous (with operand types 'two::mystruct' and
/// 'two::mystruct')clang(ovl_ambiguous_oper_binary)

/// Even though the << operators are defined in different namespace, there will be overlaod problem

// bool operator<<(two::mystruct &a, two::mystruct &b)
// {
//   std::cout << "global << operator" << std::endl;
// }

///@brief fuctions also have namespace
void f()
{
	std::cout << "f of namespace ::\n";
};

int main(int argc, char const* argv[])
{
	one::mystruct a, b;
	a.b = 2;
	b.b = 5;
	a << b;
	two::mystruct c, d;
	c.b = 2;
	d.b = 5;
	c << d;
	c >> d;

	two::f();
	return 0;
}