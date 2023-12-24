#include <iostream>

class T
{
public:
	void f() const&
	{
		std::cout << "const lvalue signature" << std::endl;
	}
	void f() &
	{
		std::cout << "non const lvalue signature" << std::endl;
	}
};

int main()
{
	T&& t = T();
	t.f();
}