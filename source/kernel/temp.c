#include <stdio.h>

struct A {
    int c;
    short d;
    char e;
} aaa;


int square(int num) {
    aaa.c = 9;
    aaa.d = 5;
}

int main() {
    square(1);
    printf("%i", aaa.c);
}