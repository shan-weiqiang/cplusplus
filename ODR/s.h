#include <iostream>
struct Trouble;

void reg(Trouble *);

struct Trouble {
    Trouble() {
        reg(this);
        std::cout << "a=" << Trouble::a++ << std::endl;
    }
    void m();
    // ODR rules that there will be only one address for a, the program either use a in lib a
    // or it use a in lib b
    static int a;
};
