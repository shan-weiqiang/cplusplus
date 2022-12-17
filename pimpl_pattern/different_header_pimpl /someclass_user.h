#include <memory>
// same calss name user can modify except those api methods
class SomeClass {

public:
  void do_some_thing();
  SomeClass();
  ~SomeClass();

private:
  char *p = nullptr;
  int age_{0};
  long long salary_{1};
  std::string name_{"suzhou zhito"};
};