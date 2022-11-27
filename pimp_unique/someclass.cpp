#include "someclass.h"
#include <iostream>

// TODO: place before definition of SomeClassImp also ok, why?
// SomeClass::~SomeClass() = default;
// SomeClass::SomeClass() = default;
class SomeClass::SomeClassImp {
public:
  void implementation() { std::cout << "implementing...\n"; }
};
void SomeClass::do_some_thing() { ptr->implementation(); }
SomeClass::~SomeClass() = default;
SomeClass::SomeClass() = default;
// following code also ok
// SomeClass::~SomeClass(){};
// SomeClass::SomeClass(){};