#include "someclass.h"

class SomeUser {
public:
  void do_some_thing() { some_.do_some_thing(); }

private:
  SomeClass some_;
};

int main() {
  SomeUser foo;
  foo.do_some_thing();
}