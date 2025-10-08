#include <stdio.h>
#include <stdlib.h>
#include "../include/combat.h"
#include "../include/joueur.h"

int main() {
    Combat_plongeur nv_plongeur_combat = {
        .nb_attaque_par_tour = NB_MAXIMUM_ATTAQUE_PAR_TOUR,
        .attaque_normale = 15,
        .attaque_special = 18,
        .competence_special = 8,
        .gestion_fatigue_vie = nouveau_plongeur()
    };
    nv_plongeur_combat.gestion_fatigue_vie.niveau_fatigue = 5;
    printf("========================\n");
    afficher_plongeur(nv_plongeur_combat.gestion_fatigue_vie);
    printf("========================\n");
    afficher_combat_plongeur(nv_plongeur_combat);
    
    int degats_calculees = combat_calcul_degats(nv_plongeur_combat.attaque_special,nv_plongeur_combat.competence_special);
    printf("Degats infligés : %d\n",degats_calculees);
    nv_plongeur_combat = combat_calcul_fatigue(nv_plongeur_combat);
    printf("Nombre d'attaque par tour : %d\n",nv_plongeur_combat.nb_attaque_par_tour);
    printf("========================\n");
    nv_plongeur_combat.gestion_fatigue_vie.niveau_oxygene = 0;
    printf("========================\n");
    nv_plongeur_combat = combat_gestion_oxygene(nv_plongeur_combat);
    printf("========================\n");
    printf("Niveau d'oxygène : %d\n",nv_plongeur_combat.gestion_fatigue_vie.niveau_oxygene);
    return 0;
}