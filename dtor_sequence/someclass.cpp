#include <iostream>

class MemberClass
{
public:
	~MemberClass();
};
MemberClass::~MemberClass()
{
	std::cout << "dtor from MemeberClass~\n";
}
class BaseClass
{
public:
	~BaseClass();
};
class SomeClass : public BaseClass
{
private:
	MemberClass m;

public:
	~SomeClass();
};

int main()
{
	SomeClass s;
	// this will first call BaseClass dtor, then MemberClass dtor, finally
	// SomeClass dtor
	s.~SomeClass();
	std::cout << "SomeClass destructor called~\n";
	exit(0);
}