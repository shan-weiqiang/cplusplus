// std::enable_shared_from_this must be inherited with public

#include <bits/types/FILE.h>
#include <memory>
#include <functional>

// must be inherit using public
class MyClass : std::enable_shared_from_this<MyClass> {
  public:
    void g() {}
    void f() {
        // if inherit std::enable_shared_from_this without public, this compiles, but throw bad_weak_ptr
        // at runtime, if c is passed to external context, such as in async context,calling c will raise
        // bad_weak_ptr error
        auto c = std::bind(&MyClass::g, shared_from_this());
    }
};

int main() {
    MyClass m;
    // bad_weak_ptr error
    m.f();
}