#include <iostream>

class T
{
public:
	void f() &&
	{
		std::cout << "non const rvalue signature" << std::endl;
	}
	void f() const&&
	{
		std::cout << "const rvalue signature" << std::endl;
	}
	void f() const&
	{
		std::cout << "const lvalue signature" << std::endl;
	}
};

int main()
{
	T().f();
}