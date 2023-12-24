#include "someclass_api.h"
#include <iostream>
#include <memory>

// class user only use methods
class SomeUser
{
public:
	void do_some_thing()
	{
		ptr->do_some_thing();
	}
	void size_of_SomeClass()
	{
		std::cout << "size of SomeClass in app: " << sizeof(*ptr) << '\n';
	}
	// the initialization of SomeClass is not only the constructor, but also the
	// memory space which is owned by this instance, in other words, the memory
	// allocation size is done during compile time and the size of this class is
	// decided using definition. since SomeClass has no data member in this case,
	// so only one byte will be allocated for the instance of SomeClass, and this
	// is decided during compile time using SomeClass definition in header file.
	//
	// However, in the user's header file SomeClass has data members and the space
	// allocated for SomeClass instance is sizeof(SomeClass)
	//
	// Even though methods can be linked from user's shared lib, but as long as
	// the methods use any user's data member,there will be memory access problem
	// because the instance actually is only one byte.
	//
	// This is why pimp design need a opaque pointer
	//
	// With opaque pointer, the user only kown a forward declaration of Impl
	// class, the actual size and initialzation of instance of Impl is all in the
	// shared lib. On the contrary, as long as the user konw the definition of
	// Impl class, user has to be recompiled when Impl class changes.
	//
	// Opaque is the key here.
	//
	// One possible way and has been tested negtive is to allocate manually the
	// memory needed in the implmentation side inside ctor at the address given by
	// the user side. However, because the user's instance only contain one byte,
	// we can not be sure that the memory bytes following this address are all
	// available for use.
	SomeUser()
		: ptr(new SomeClass())
	{ }

private:
	std::shared_ptr<SomeClass> ptr;
};

int main()
{
	SomeUser user;
	// this compiles and runs with danger, because in current binary file
	// SomeClass without any data member is initialized in the heap however, in
	// the shared lib, do_som_thing function will use data members of this
	// instance, which we do not have.
	user.do_some_thing();
	user.size_of_SomeClass();
}