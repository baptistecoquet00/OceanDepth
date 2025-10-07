#include <stdio.h>
#include <assert.h>
#include "../../include/fonctions.h"

void test_addition() {
    printf("Test addition... ");
    assert(addition(2, 3) == 5);
    assert(addition(-1, 1) == 0);
    printf("✓ OK\n");
}

void test_multiplication() {
    printf("Test multiplication... ");
    assert(multiplication(2, 3) == 6);
    assert(multiplication(0, 5) == 0);
    printf("✓ OK\n");
}

int main() {
    printf("Début des tests...\n");
    test_addition();
    test_multiplication();
    printf("✅ Tous les tests sont passés !\n");
    return 0;
}