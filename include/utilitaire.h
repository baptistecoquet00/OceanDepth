#ifndef OK_H
#define OK_H

#include <stdio.h>

void ok(){
    printf("\e[1;32mOK\e[0m\n");
}

void tous_test_passe(){
    printf("\e[1;32mTous les tests sont passés !\e[0m\n");
}

void debut_test(){
    printf("Début des tests...\n");
}

void separateur_sections(){
    printf("========================\n");
}

#endif