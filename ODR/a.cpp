#include <iostream>
#include "s.h"
#include <stdio.h>
void f() { std::cout << "from a" << std::endl; }

// lib a depend on static lib s:
// 1. lib a will consume libs, including all code inside the translation unit s.cpp
//      Note that even though dummy.cpp is also in lib s, it will not be copied into
//      lib a, since lib a does not depend on dummy.cpp and static libraries are copied
//      in the unit of object files.
// 2. lib a will contain global variable: Trouble t
void a_trouble(Trouble *t) {
    t->m();
}

// even without this line, t is copied into lib a

extern Trouble t;

void __attribute__((constructor))a_print() {
    // This will print the same result in lib b, the logic are:
    // 1. Both dynamic symbols of Trouble::m and Trouble t exist in lib a and lib b
    // 2. Both lib a and lib b contain code of Trouble::m and constructor
    // 3. But at runtime ODR rule applies, the dynamic linker assures that there
    //    will be only one address for one symbol; So for Trouble::m, the linker will
    //    find the address either in lib a or in lib b; for Trouble t, there are two
    //    instances both in lib a and in lib b, the initialization code for both lib a
    //    and lib b will call constructor of Trouble for t, however, ODR implies that
    //    there must be only one global unique address for the same name, here 't', so
    //    lib a and lib b will both construct t individually at the same memory address(
    //     address of t).
    printf("%p\n", &Trouble::m);
    printf("%p\n", &t);
}

// 1. test if multi definition in different tu can be compiled into one static lib?
// 2. test if visibility attribute applies both to symbol consumer and provider
