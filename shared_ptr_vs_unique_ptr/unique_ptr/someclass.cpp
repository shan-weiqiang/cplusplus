#include "someclass.h"

#include <iostream>

// INFO: It's ok to put dtor before definition of SomeClassImp, even here SomeClassImp
// is still incomplete type. This compiles becuause std::unique_ptr is template, so the
// two phase namelook up here applies. The actual initialization of the dtor of the unique_ptr
// will be at the end of the translation unit. So there will be no incomplete type problem.

// SomeClass::~SomeClass() = default;
// SomeClass::SomeClass() = default;

SomeClass::~SomeClass()
{
	// This testifies that print_size function is initialized after the complete definition of
	// SomeClassImp
	this->size.print_size();
};
SomeClass::SomeClass()
{
	this->size.print_size();
};
class SomeClass::SomeClassImp
{
public:
	void implementation()
	{
		std::cout << "implementing...\n";
	}
};
void SomeClass::do_some_thing()
{
	ptr->implementation();
}

template <typename T>
void SomeTemplateClass<T>::print_size()
{
	std::cout << sizeof(T) << std::endl;
}
// SomeClass::~SomeClass() = default;
// SomeClass::SomeClass() = default;
// following code also ok
// SomeClass::~SomeClass(){};
// SomeClass::SomeClass(){};