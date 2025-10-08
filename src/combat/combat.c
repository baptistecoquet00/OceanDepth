#include <stdio.h>
#include <stdlib.h>
#include "../../include/creatures.h"
#include "../../include/combat.h"

void afficher_combat_plongeur(Combat_plongeur combat_plongeur){
    printf("nombre d'attaque par tour : %d\n",combat_plongeur.nb_attaque_par_tour);
    printf("Attaque normale : %d\n",combat_plongeur.attaque_normale);
    printf("Attque spéciale : %d\n",combat_plongeur.attaque_special);
    printf("Competence spéciale : %d\n",combat_plongeur.competence_special);
    printf("========================\n");
    afficher_plongeur(combat_plongeur.gestion_fatigue_vie);
   
}

int combat_calcul_degats(int attaque_joueur, int bonus_arme){
    //CreatureMarine creature = nouvelle_creature();
    int defense_creature = 10;
    int degats = (attaque_joueur + bonus_arme) - defense_creature;
    if(degats < 1) degats = 1; 
    return degats;
}

Combat_plongeur combat_calcul_fatigue(Combat_plongeur plongeur_combat){
    int calcul_fatigue = plongeur_combat.gestion_fatigue_vie.niveau_fatigue; 
    if(calcul_fatigue >= FATIGUE_NV_ZERO && calcul_fatigue <= FATIGUE_NV_UN){
        plongeur_combat.nb_attaque_par_tour = NB_MAXIMUM_ATTAQUE_PAR_TOUR;
    }else if(calcul_fatigue >= FATIGUE_NV_DEUX && calcul_fatigue <= FATIGUE_NV_TROIS){
        plongeur_combat.nb_attaque_par_tour = NB_MOYEN_ATTAQUE_PAR_TOUR;
    }else if(calcul_fatigue >= FATIGUE_NV_QUATRE && calcul_fatigue <=FATIGUE_NV_CINQ){
        plongeur_combat.nb_attaque_par_tour = NB_BAS_ATTAQUE_PAR_TOUR;
    }

    return plongeur_combat;
}

//TODO
Combat_plongeur combat_gestion_vie(int points_de_vie);

inline int attaque_normal(Combat_plongeur plongeur_combat){
    plongeur_combat.gestion_fatigue_vie.niveau_oxygene -= 2;
    return plongeur_combat.gestion_fatigue_vie.niveau_oxygene;
}

inline int competence_speciale(Combat_plongeur plongeur_combat){
    plongeur_combat.gestion_fatigue_vie.niveau_oxygene -= 5;
    return plongeur_combat.gestion_fatigue_vie.niveau_oxygene;
}

Combat_plongeur combat_gestion_oxygene(Combat_plongeur plongeur_combat){

    if(plongeur_combat.attaque_normale){
        plongeur_combat.gestion_fatigue_vie.niveau_oxygene += attaque_normal(plongeur_combat);
    }

    if(plongeur_combat.competence_special){
        plongeur_combat.gestion_fatigue_vie.niveau_oxygene += competence_speciale(plongeur_combat);
    }
    
    plongeur_combat.gestion_fatigue_vie.niveau_oxygene = oxygene_vide(plongeur_combat.gestion_fatigue_vie);
    oxygene_critique(plongeur_combat.gestion_fatigue_vie);

    while (plongeur_combat.gestion_fatigue_vie.niveau_oxygene <= 0)
    {
        plongeur_combat.gestion_fatigue_vie.points_de_vie -=5;
        //printf("Point de vie avant la mort : %d\n",plongeur_combat.gestion_fatigue_vie.points_de_vie);
        if(plongeur_combat.gestion_fatigue_vie.points_de_vie <= 0){
            est_mort(plongeur_combat.gestion_fatigue_vie); 
            //affiche_est_mort();
            break;
        }
    }
    afficher_plongeur(plongeur_combat.gestion_fatigue_vie);
    return plongeur_combat;
}

