#include <iostream>
#include <stdio.h>

void f(){
    printf("from main\n ");
}

extern int a;

int main(){
 printf("hello%d\n", a);
}