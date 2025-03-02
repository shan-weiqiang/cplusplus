#include <iostream>
#include "s.h"
void f() { std::cout << "from b" << std::endl; }
void g() { std::cout << "from b::g" << std::endl; }

// lib b depend on static lib s:
// 1. lib b will consume libs, including all code inside the translation unit
//      Note that lib b depend on dummy in lib s, so object code of dummy will
//      be copied into lib b, which will bring Dummy d global variable into app
// 2. lib b will contain global variable: Trouble t
void b_trouble(Trouble *t) {
    t->m();
}

// even without this line, t is copied into lib b
extern Trouble t;

void __attribute__((constructor)) b_print() {
    printf("%p\n", &Trouble::m);
    printf("%p\n", &t);
}

// Make lib b depend on dummy in lib s
struct Dummy {
    Dummy() {
        std::cout << "Dummy constructor called" << std::endl;
    }
    void f();
};

void d(Dummy d) {
    d.f();
}
