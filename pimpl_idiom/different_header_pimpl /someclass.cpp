#include <iostream>

#include "someclass_user.h"

void SomeClass::do_some_thing()
{
	std::cout << "Implementations...\n"
			  << "name: " << this->name_ << '\n'
			  << "age: " << this->age_ << '\n'
			  << "salary: " << this->salary_ << '\n'
			  << "size of SomeClass in shared lib: " << sizeof(*this) << '\n';
}
SomeClass::SomeClass()
{
	p = new(this) char[sizeof(*this)];

	age_ = 23;
	salary_ = 454444;
};
SomeClass::~SomeClass()
{
	delete[] p;
}