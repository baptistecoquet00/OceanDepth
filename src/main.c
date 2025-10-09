#include <stdio.h>
#include <stdlib.h>
#include "../include/combat.h"
#include "../include/joueur.h"
#include "../include/utilitaire.h"
#include "../include/interface_combat.h"

int main() {
    CreatureMarine tab[5];
    int nb = 5;   

    generer_creatures(tab, &nb, 100); 

    // for (int i = 0; i < nb; i++) {
    //     afficher_creature(&tab[i]);
    // }

    Combat_plongeur nv_plongeur_combat = {
        .nb_attaque_par_tour = NB_MAXIMUM_ATTAQUE_PAR_TOUR,
        .attaque_normale = 15,
        .attaque_special = 18,
        .competence_special = 8,
        .gestion_fatigue_vie = nouveau_plongeur()
    };
    
    nv_plongeur_combat.gestion_fatigue_vie.niveau_fatigue = 5;
    separateur_sections();
    afficher_plongeur(nv_plongeur_combat.gestion_fatigue_vie);
    
    separateur_sections();
    afficher_combat_plongeur(nv_plongeur_combat);
    int degats_calculees = combat_calcul_degats(nv_plongeur_combat.attaque_special,nv_plongeur_combat.competence_special);
    printf("Degats infligés : %d\n",degats_calculees);
    nv_plongeur_combat = combat_calcul_fatigue(nv_plongeur_combat);
    printf("Nombre d'attaque par tour : %d\n",nv_plongeur_combat.nb_attaque_par_tour);
    
    separateur_sections();
    nv_plongeur_combat.gestion_fatigue_vie.niveau_oxygene = 0;
    nv_plongeur_combat = combat_gestion_oxygene(nv_plongeur_combat);
    
    if(nv_plongeur_combat.gestion_fatigue_vie.niveau_oxygene == ZERO_OXYGENE){
        oxygene_critique(nv_plongeur_combat.gestion_fatigue_vie);
    }

    if(nv_plongeur_combat.gestion_fatigue_vie.points_de_vie <= 0){
        affiche_est_mort();
    }

    afficher_plongeur(nv_plongeur_combat.gestion_fatigue_vie);
    
    separateur_sections();
    printf("Niveau d'oxygène : %d\n",nv_plongeur_combat.gestion_fatigue_vie.niveau_oxygene);
    afficher_interface_combat();
    return 0;
}