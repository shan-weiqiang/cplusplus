// when std::bind is used to bind a member function, the first parameter can be raw pointer to the class, or:

// As described in Callable, when invoking a pointer to non-static member function or pointer to non-static data member,
// the first argument has to be a reference or pointer (including, possibly, smart pointer such as std::shared_ptr and std::unique_ptr)
// to an object whose member will be accessed.

// https://en.cppreference.com/w/cpp/utility/functional/bind
// https://stackoverflow.com/questions/54417757/how-is-this-shared-ptr-automatically-converted-to-a-raw-pointer

// The mechanism behind this behavior is that:
// 1. after C++ 11, std::mem_fn is used to invoke the function and can support pass shared ptr as first argument
// 2. after C++ 17, std::invoke is used to invoke the function and also support this behavior

#include <functional>
#include <iostream>
#include <memory>

class Example : public std::enable_shared_from_this<Example> {
  public:
    void some_func() {
        std::cout << "some" << std::endl;
    }
};

int main() {
    auto a = std::make_shared<Example>();
    std::function<void()> f = std::bind(&Example::some_func, a->shared_from_this());

    f();
}