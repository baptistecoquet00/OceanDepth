#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "../include/utilitaire.h"

#define ARENE_IMPLEMENTATION

#include "../include/gm.h"



void test_nouvelle_arene(){
    Arene* arene_null = nouvelle_arene(0);
    Arene* arene_non_null = nouvelle_arene(32);

    printf("Test pour la creation d'arene... ");
    
    assert(arene_null == NULL);
    assert(arene_non_null != NULL);
    assert(arene_non_null->taille_total == 32);
    assert(arene_non_null->index == 0);    
    arene_detruite(arene_null);
    arene_detruite(arene_non_null);
    ok();

    

}

void test_etendre_arene(){
    Arene* arene = nouvelle_arene(6);
    char* ptr = arene_allocation(arene,6);
    arene = etendre_arene(arene,12);
    memcpy(ptr, "Hello\0", 6);

    printf("Test pour etendre l'arene... ");
    
    assert(arene->taille_total == 12);
    assert(arene->index == 6);

    assert(etendre_arene(NULL,0)==NULL);
    assert(etendre_arene(arene,0)==NULL);
    assert(etendre_arene(NULL,13)==NULL);
    arene_detruite(arene);
    ok();

    
}

void test_arene_allocation(){
    Arene* arene = nouvelle_arene(13 + sizeof(long) * 3+3);
    char* tableau_char = arene_allocation(arene,13);
    long* tableau_long;
    long tablea_attendu_long[3] = {999,9999,99999};

    printf("Test pour l'allocation d'arene... ");
    
    assert(arene_allocation(arene,0)==NULL);
    memcpy(tableau_char,"Hello, world!", 13);
    assert(strcmp(tableau_char, "Hello, world!") == 0);
    assert(arene->index == 13);      

    tableau_long = arene_allocation(arene,sizeof(long)*3);
    memcpy(tableau_long,tablea_attendu_long,sizeof(long)*3);
    assert(tableau_long[0] == 999);
    assert(tableau_long[1] == 9999); 
    assert(tableau_long[2] == 99999);
    assert(arene->index==arene->taille_total); //40

    ok();

    arene_detruite(arene);
}

void test_arene_allocation_alignee(){
    Arene* arene = nouvelle_arene(64);

    printf("Test pour l'allocation d'arene alignée... ");
    assert(arene_allocation_alignee(arene,0,0)==NULL);
    
    arene_allocation_alignee(arene,8,4);
    assert(arene->index == 8);

    arene_allocation_alignee(arene,3,4);
    assert(arene->index == 11);

    arene_allocation_alignee(arene,12,4);
    assert(arene->index == 24);

    arene_allocation_alignee(arene,3,4);
    
    assert(arene->index == 27);

    arene_allocation_alignee(arene,1,4);
    assert(arene->index == 29);

    assert(arene_allocation_alignee(arene,100,0)==NULL);
    ok();
    arene_detruite(arene);
}

int main(){
    debut_test();
    test_nouvelle_arene();
    test_etendre_arene();
    test_arene_allocation();
    test_arene_allocation_alignee();
    tous_test_passe();
}