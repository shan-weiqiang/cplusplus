#include "someclass.h"
#include <iostream>

struct SomeClass::SomeClassImpl
{
	void impl_()
	{
		std::cout << "Implementations...\n"
				  << "name: " << name_ << '\n'
				  << "age: " << age_ << '\n'
				  << "salary: " << salary_ << '\n';
	}
	std::string name_ = "Tom";
	int age_ = 0;
	double salary_ = 0.0;
};

void SomeClass::do_some_thing()
{
	ptr->impl_();
}
SomeClass::SomeClass()
	: ptr(new SomeClassImpl())
{ }
SomeClass::~SomeClass(){};