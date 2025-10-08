#include <stdio.h>
#include <assert.h>
#include "../include/joueur.h"
#include "../include/ok.h"

void test_etat_perles(){
    Plongeur plongeur = nouveau_plongeur();
    plongeur.perles = 100;
    printf("Test pour le nombre de perles... ");
    assert(etat_perles(plongeur)==100);
    ok();
}

void test_est_mort(){
    Plongeur plongeur_mort = nouveau_plongeur();
    plongeur_mort.points_de_vie = 0;
    printf("Test pour voir si le plongeur est mort... ");
    assert(est_mort(plongeur_mort)==0);
    assert(est_mort(plongeur_mort)!=-1);
    plongeur_mort.points_de_vie = 100;
    assert(est_mort(plongeur_mort)==-1);
    ok();
}

void test_vie_maximum(){
    Plongeur plongeur_vie_max = nouveau_plongeur();
    printf("Test de la vie maximum... ");
    assert(vie_maximum(plongeur_vie_max)== 0);
    plongeur_vie_max.points_de_vie = 50;
    assert(vie_maximum(plongeur_vie_max)!= 0);
    ok();
}

int main() {
    debut_test();
    test_etat_perles();
    test_est_mort();
    test_vie_maximum();
    tous_test_passe();
    return 0;
}