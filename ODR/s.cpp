
#include "iostream"
#include "s.h"


int Trouble::a = 0;
void Trouble::m() {
    std::cout << "from Trouble::m" << std::endl;
}

// global variable
Trouble t;
