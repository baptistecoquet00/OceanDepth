#include <stdio.h>
#include <stdlib.h>
#include "../../include/creatures.h"
#include "../../include/combat.h"
#include "../../include/tour_par_tour.h"


Combat_plongeur* nouveau_combat_plongeur(Plongeur* plongeur){
    Combat_plongeur* plongeur_combat = (Combat_plongeur*)malloc(sizeof(Combat_plongeur));
    plongeur_combat->nb_attaque_par_tour = NB_MAXIMUM_ATTAQUE_PAR_TOUR;
    plongeur_combat->attaque_normale = 0;
    plongeur_combat->attaque_special = 0;
    plongeur_combat->competence_special = 0;
    plongeur_combat->gestion_fatigue_vie = plongeur;
    return plongeur_combat;
}

void afficher_combat_plongeur(Combat_plongeur* combat_plongeur){
    printf("nombre d'attaque par tour : %d\n",combat_plongeur->nb_attaque_par_tour);
    printf("Attaque normale : %d\n",combat_plongeur->attaque_normale);
    printf("Attque spéciale : %d\n",combat_plongeur->attaque_special);
    printf("Competence spéciale : %d\n",combat_plongeur->competence_special);
    printf("========================\n");
    afficher_plongeur(combat_plongeur->gestion_fatigue_vie);
   
}

int combat_calcul_degats(int attaque_joueur, int bonus_arme){
    //CreatureMarine creature = nouvelle_creature();
    CreatureMarine creature;
    creature.defense = 10;
    int defense_creature = creature.defense;
    int degats = (attaque_joueur + bonus_arme) - defense_creature;
    if(degats < 1) degats = 1; 
    return degats;
}

void combat_plongeur_calcul_fatigue(Combat_plongeur* plongeur_combat){
    int calcul_fatigue = plongeur_combat->gestion_fatigue_vie->niveau_fatigue; 
    
    if(calcul_fatigue >= FATIGUE_NV_ZERO && calcul_fatigue <= FATIGUE_NV_UN)
    {
        plongeur_combat->nb_attaque_par_tour = NB_MAXIMUM_ATTAQUE_PAR_TOUR;
    }
    else if(calcul_fatigue >= FATIGUE_NV_DEUX && calcul_fatigue <= FATIGUE_NV_TROIS)
    {
        plongeur_combat->nb_attaque_par_tour = NB_MOYEN_ATTAQUE_PAR_TOUR;
    }
    else if(calcul_fatigue >= FATIGUE_NV_QUATRE && calcul_fatigue <=FATIGUE_NV_CINQ)
    {
        plongeur_combat->nb_attaque_par_tour = NB_BAS_ATTAQUE_PAR_TOUR;
    }

    return;
    //return plongeur_combat;
}

void combat_plongeur_gestion_vie(Combat_plongeur* plongeur_combat, CreatureMarine* creature){
    
    if(creature->attaque_maximale) 
    {
        plongeur_combat->gestion_fatigue_vie->points_de_vie -= creature->attaque_maximale;
    }
    
    if(creature->attaque_minimale)
    {
        plongeur_combat->gestion_fatigue_vie->points_de_vie -= creature->attaque_minimale;
    }

    if(est_mort(plongeur_combat->gestion_fatigue_vie)==0){
        return;
        //return plongeur_combat;    
    }
    return;
    //return plongeur_combat;
}

inline int attaque_normal(Combat_plongeur* plongeur_combat){
    plongeur_combat->gestion_fatigue_vie->niveau_oxygene -= 2;
    return plongeur_combat->gestion_fatigue_vie->niveau_oxygene;
}

inline int competence_speciale(Combat_plongeur* plongeur_combat){
    plongeur_combat->gestion_fatigue_vie->niveau_oxygene -= 5;
    return plongeur_combat->gestion_fatigue_vie->niveau_oxygene;
}

void combat_plongeur_gestion_oxygene(Combat_plongeur* plongeur_combat){

    if(plongeur_combat->attaque_normale){
        plongeur_combat->gestion_fatigue_vie->niveau_oxygene += attaque_normal(plongeur_combat);
    }

    if(plongeur_combat->competence_special){
        plongeur_combat->gestion_fatigue_vie->niveau_oxygene += competence_speciale(plongeur_combat);
    }
    
    plongeur_combat->gestion_fatigue_vie->niveau_oxygene = oxygene_vide(plongeur_combat->gestion_fatigue_vie);
    //oxygene_critique(plongeur_combat.gestion_fatigue_vie);

    while (plongeur_combat->gestion_fatigue_vie->niveau_oxygene <= 0)
    {
        plongeur_combat->gestion_fatigue_vie->points_de_vie -=5;
        //printf("Point de vie avant la mort : %d\n",plongeur_combat.gestion_fatigue_vie.points_de_vie);
        
        if(est_mort(plongeur_combat->gestion_fatigue_vie)==0){
            break;
        }
    }
    return;

    //afficher_plongeur(plongeur_combat.gestion_fatigue_vie);
    //printf("Niveau d'oxygène : %d\n",plongeur_combat.gestion_fatigue_vie.niveau_oxygene);
    //return plongeur_combat;
}

// void Systeme_combat(Combat_plongeur *plongeur_combat){
//     TourJoueur tour;
//     tour_init(&tour);

//     while (/* combat actif */) {
        
//         if (tour_est_au_joueur(&tour)) {
//             // Afficher interface "En attente de votre action..."
            
//             if (tour_attente_action(&tour)) {
//                 // Le joueur n'a pas encore agi
//                 // Afficher les boutons/bindings d'action
//                 printf("Choisissez une action...\n");
                
//             } else {
//                 // Le joueur a agi, on passe à l'ennemi
//                 tour_passer_ennemi(&tour);
//             }
            
//         } else {
//             // Tour de l'ennemi
//             executer_ia_ennemi();
            
//             // Après l'ennemi, on passe au tour suivant
//             tour_suivant(&tour);
//         }
//     }
// }

void free_combat_plongeur(Combat_plongeur* plongeur_combat){
    if (plongeur_combat != NULL) {        
        free(plongeur_combat);
    }
}
