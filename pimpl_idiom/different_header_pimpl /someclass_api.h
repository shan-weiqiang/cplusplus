#include <memory>

// definition decide the memory size during compile time
class SomeClass {
  // only include api used by middleware
public:
  void do_some_thing();
  SomeClass();
  ~SomeClass();
};