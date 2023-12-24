#include <memory>
class SomeClass
{

public:
	void do_some_thing();
	SomeClass();
	~SomeClass();

private:
	// here the SomeClassImpl only has a declaration, not definition. It is
	// 'opaque'. This is why the user need not to compile again. The interesting
	// thing is that since it is nested inside a pointer or shared_ptr, the memroy
	// layout of SomeClass is determined.
	//
	// User class konws at compile time the memory needed by SomeClassImpl and
	// give it during runtime and the rest of initialization work is all done from
	// the shared lib.
	//
	// This way, whatever happens to SomeClassImpl do not influence user, the user
	// only need to know the api of SomeClass.
	//
	// At bottom of pimpl design pattern, it a story of memory allocation by user
	// and initiaization(use) by shared lib.

	struct SomeClassImpl;
	/// here if use std::share_ptr there is no need to define ctor and dtor after
	/// definition of SomeClassImpl buf if use unique_ptr, ctor and dtor must be
	/// defined after definition of SomeClassImpl; The reason behind see
	/// 'pimpl_unique' folder
	std::unique_ptr<SomeClassImpl> ptr;
};