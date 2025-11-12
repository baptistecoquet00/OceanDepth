#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/combat.h"
#include "../include/joueur.h"
#include "../include/utilitaire.h"
#include "../include/creatures.h"

void test_combat_calcul_degats() {
    // Combat_plongeur nv_plongeur_combat = {
    //     .nb_attaque_par_tour = NB_MAXIMUM_ATTAQUE_PAR_TOUR,
    //     .attaque_normale = 15,
    //     .attaque_special = 18,
    //     .competence_special = 8,
    //     .gestion_fatigue_vie = nouveau_plongeur()
    // };
    Plongeur* nv_plongeur = nouveau_plongeur();
    Combat_plongeur* nv_plongeur_combat = nouveau_combat_plongeur(nv_plongeur);
    nv_plongeur_combat->attaque_special = 18;
    nv_plongeur_combat->competence_special = 8;
    int degats_calculees = combat_calcul_degats(nv_plongeur_combat->attaque_special,nv_plongeur_combat->competence_special);
    printf("Test pour le calcul de degats... ");
    assert(degats_calculees==16);
    ok();
}

void test_combat_calcul_fatigue(){
    // Combat_plongeur nv_plongeur_combat = {
    //     .nb_attaque_par_tour = NB_MAXIMUM_ATTAQUE_PAR_TOUR,
    //     .attaque_normale = 15,
    //     .attaque_special = 18,
    //     .competence_special = 8,
    //     .gestion_fatigue_vie = nouveau_plongeur()
    // };
    
    Plongeur* nv_plongeur = nouveau_plongeur();
    Combat_plongeur* nv_plongeur_combat = nouveau_combat_plongeur(nv_plongeur);

    printf("Test pour le calcul de fatigue... ");
    
    combat_plongeur_calcul_fatigue(nv_plongeur_combat);
    assert(nv_plongeur_combat->nb_attaque_par_tour==3);

    
    nv_plongeur_combat->gestion_fatigue_vie->niveau_fatigue = 2;
    combat_plongeur_calcul_fatigue(nv_plongeur_combat);
    assert(nv_plongeur_combat->nb_attaque_par_tour==2);
    
    nv_plongeur_combat->gestion_fatigue_vie->niveau_fatigue = 5;
    combat_plongeur_calcul_fatigue(nv_plongeur_combat);
    assert(nv_plongeur_combat->nb_attaque_par_tour==1);

    ok();
}

// TODO
void test_combat_calcul_oxygene(){
    // Combat_plongeur nv_plongeur_combat = {
    //     .nb_attaque_par_tour = NB_MAXIMUM_ATTAQUE_PAR_TOUR,
    //     .attaque_normale = 15,
    //     .attaque_special = 18,
    //     .competence_special = 8,
    //     .gestion_fatigue_vie = nouveau_plongeur()
    // };
    Plongeur* nv_plongeur = nouveau_plongeur();
    Combat_plongeur* nv_plongeur_combat = nouveau_combat_plongeur(nv_plongeur);
    
    nv_plongeur_combat->gestion_fatigue_vie->niveau_oxygene = ZERO_OXYGENE;
    
    combat_plongeur_gestion_oxygene(nv_plongeur_combat);
    
    printf("Test pour le calcul d'oxygene... ");
    assert(nv_plongeur_combat->gestion_fatigue_vie->points_de_vie==0);
    assert(nv_plongeur_combat->gestion_fatigue_vie->niveau_oxygene==0);
    ok();
}

void test_combat_calcul_vie(){
    // Combat_plongeur nv_plongeur_combat = {
    //     .nb_attaque_par_tour = NB_MAXIMUM_ATTAQUE_PAR_TOUR,
    //     .attaque_normale = 15,
    //     .attaque_special = 18,
    //     .competence_special = 8,
    //     .gestion_fatigue_vie = nouveau_plongeur()
    // };

    Plongeur* nv_plongeur = nouveau_plongeur();
    Combat_plongeur* nv_plongeur_combat = nouveau_combat_plongeur(nv_plongeur);

    CreatureMarine* creature = (CreatureMarine*)malloc(sizeof(CreatureMarine));
    creature->attaque_maximale = 10;
    creature->attaque_minimale = 5;
    combat_plongeur_gestion_vie(nv_plongeur_combat,creature);
    printf("Test pour le calcul de vie... ");
    assert(nv_plongeur_combat->gestion_fatigue_vie->points_de_vie==nv_plongeur_combat->gestion_fatigue_vie->points_de_vie_max-15);
    ok();
}

// test combat
int main() {
    debut_test();
    test_combat_calcul_degats();
    test_combat_calcul_fatigue();
    test_combat_calcul_oxygene();
    test_combat_calcul_vie();
    tous_test_passe();
    return 0;
}