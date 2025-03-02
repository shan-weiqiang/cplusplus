#include <iostream>

int a=0;

// Following line will produce link error:
// __attribute__((visibility("hidden"))) will stop external linkage of f(), both not providing it to outside, and not finding it from outside

__attribute__((visibility("hidden")))void f();

void __attribute__((constructor))g(){
    f();
}