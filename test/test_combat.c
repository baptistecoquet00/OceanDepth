#include <stdio.h>
#include <assert.h>
#include "../include/combat.h"
#include "../include/joueur.h"
#include "../include/ok.h"

void test_combat_calcul_degats() {
    Combat_plongeur nv_plongeur_combat = {
        .nb_attaque_par_tour = NB_MAXIMUM_ATTAQUE_PAR_TOUR,
        .attaque_normale = 15,
        .attaque_special = 18,
        .competence_special = 8,
        .gestion_fatigue_vie = nouveau_plongeur()
    };
    int degats_calculees = combat_calcul_degats(nv_plongeur_combat.attaque_special,nv_plongeur_combat.competence_special);
    printf("Test pour le calcul de degats... ");
    assert(degats_calculees==16);
    ok();
}

void test_combat_calcul_fatigue(){
    Combat_plongeur nv_plongeur_combat = {
        .nb_attaque_par_tour = NB_MAXIMUM_ATTAQUE_PAR_TOUR,
        .attaque_normale = 15,
        .attaque_special = 18,
        .competence_special = 8,
        .gestion_fatigue_vie = nouveau_plongeur()
    };
    
    printf("Test pour le calcul de fatigue... ");
    
    nv_plongeur_combat = combat_calcul_fatigue(nv_plongeur_combat);
    assert(nv_plongeur_combat.nb_attaque_par_tour==3);

    
    nv_plongeur_combat.gestion_fatigue_vie.niveau_fatigue = 2;
    nv_plongeur_combat = combat_calcul_fatigue(nv_plongeur_combat);
    assert(nv_plongeur_combat.nb_attaque_par_tour==2);
    
    nv_plongeur_combat.gestion_fatigue_vie.niveau_fatigue = 5;
    nv_plongeur_combat = combat_calcul_fatigue(nv_plongeur_combat);
    assert(nv_plongeur_combat.nb_attaque_par_tour==1);

    ok();
}

// TODO
void test_combat_calcul_oxygene(){
     Combat_plongeur nv_plongeur_combat = {
        .nb_attaque_par_tour = NB_MAXIMUM_ATTAQUE_PAR_TOUR,
        .attaque_normale = 15,
        .attaque_special = 18,
        .competence_special = 8,
        .gestion_fatigue_vie = nouveau_plongeur()
    };
    printf("Test pour le calcul d'oxygene... ");

    ok();
}

// test combat
int main() {
    debut_test();
    test_combat_calcul_degats();
    test_combat_calcul_fatigue();
    test_combat_calcul_oxygene();
    tous_test_passe();
    return 0;
}