#include <stdio.h>
#include <assert.h>
#include "../include/fonctions.h"
#include "../include/ok.h"

void test_addition() {
    printf("Test addition... ");
    assert(addition(2, 3) == 5);
    assert(addition(-1, 1) == 0);
    ok();
}

void test_multiplication() {
    printf("Test multiplication... ");
    assert(multiplication(2, 3) == 6);
    assert(multiplication(0, 5) == 0);
    ok();
}

int main() {
    debut_test();
    test_addition();
    test_multiplication();
    tous_test_passe();
    return 0;
}