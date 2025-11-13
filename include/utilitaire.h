#ifndef OK_H
#define OK_H

#include <stdio.h>

void ok(){
    printf("\e[1;32mOK\e[0m\n");
}

void tous_test_passe(){
    printf("Tous les tests sont passes!\n");
}

void debut_test(){
    printf("Debut des tests...\n");
}

void separateur_sections(){
    printf("========================\n");
}

#endif