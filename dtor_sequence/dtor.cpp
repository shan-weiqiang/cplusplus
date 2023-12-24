#include <iostream>
class MemberClass
{
public:
	~MemberClass();
};

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

// even though here user defined ~SomeClass, compiler will insert base class and
// member instance dtor calls inside this dtor
SomeClass::~SomeClass()
{
	std::cout << "dtor from SomeClass~\n";
}
BaseClass::~BaseClass()
{
	std::cout << "dtor from BaseClass~\n";
}
