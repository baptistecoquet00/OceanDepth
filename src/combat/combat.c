#include <stdio.h>
#include <stdlib.h>

#include "../../include/gm.h"
#include "../../include/creatures.h"
#include "../../include/tour_par_tour.h"
#include "../../include/combat.h"


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

void combat_plongeur_calcul_fatigue(Combat_plongeur* plongeur_combat, int nombre_attaques_ce_tour) {
    // Ajouter le nombre d'attaques de ce tour au compteur
    plongeur_combat->compteur_attaques += nombre_attaques_ce_tour;
    
    // Toutes les 3 attaques cumulées, ajouter 1 point de fatigue
    while (plongeur_combat->compteur_attaques >= 3) {
        if (plongeur_combat->gestion_fatigue_vie->niveau_fatigue < FATIGUE_NV_CINQ) {
            plongeur_combat->gestion_fatigue_vie->niveau_fatigue++;
            printf(">>> Fatigue +1 (%d attaques cumulees)\n", plongeur_combat->compteur_attaques);
        }
        plongeur_combat->compteur_attaques -= 3; // On retire 3 du compteur
    }
    
    // Calcul du nombre d'attaques par tour basé sur la fatigue actuelle
    int calcul_fatigue = plongeur_combat->gestion_fatigue_vie->niveau_fatigue; 
    
    if(calcul_fatigue >= FATIGUE_NV_ZERO && calcul_fatigue <= FATIGUE_NV_UN) {
        plongeur_combat->nb_attaque_par_tour = NB_MAXIMUM_ATTAQUE_PAR_TOUR;
    }
    else if(calcul_fatigue >= FATIGUE_NV_DEUX && calcul_fatigue <= FATIGUE_NV_TROIS) {
        plongeur_combat->nb_attaque_par_tour = NB_MOYEN_ATTAQUE_PAR_TOUR;
    }
    else if(calcul_fatigue >= FATIGUE_NV_QUATRE && calcul_fatigue <= FATIGUE_NV_CINQ) {
        plongeur_combat->nb_attaque_par_tour = NB_BAS_ATTAQUE_PAR_TOUR;
    }
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
        plongeur_combat->gestion_fatigue_vie->niveau_oxygene -= 2;
    }

    if(plongeur_combat->competence_special){
        plongeur_combat->gestion_fatigue_vie->niveau_oxygene -= 5;
    }
    
    plongeur_combat->gestion_fatigue_vie->niveau_oxygene = oxygene_vide(plongeur_combat->gestion_fatigue_vie);
    //oxygene_critique(plongeur_combat.gestion_fatigue_vie);

    if(plongeur_combat->gestion_fatigue_vie->niveau_oxygene <= 0)
    {
        plongeur_combat->gestion_fatigue_vie->points_de_vie -=5;
        //printf("Point de vie avant la mort : %d\n",plongeur_combat.gestion_fatigue_vie.points_de_vie);
        
        if(est_mort(plongeur_combat->gestion_fatigue_vie)==0){
           return;
        }
    }
    return;

    //afficher_plongeur(plongeur_combat.gestion_fatigue_vie);
    //printf("Niveau d'oxygène : %d\n",plongeur_combat.gestion_fatigue_vie.niveau_oxygene);
    //return plongeur_combat;
}

// Calculer les dégâts d'une créature (aléatoire entre min et max)
int calculer_degats_creature(CreatureMarine *creature) {
    return creature->attaque_minimale + 
           rand() % (creature->attaque_maximale - creature->attaque_minimale + 1);
}

// Appliquer les effets spéciaux
void appliquer_effet_special(CreatureMarine *creature, Combat_plongeur *plongeur,Fenetre *combat_win) {
    if (strcmp(creature->effet_special, "poison") == 0) {
        // Logique poison
        plongeur->gestion_fatigue_vie->points_de_vie -= 3;
        afficher_fenetre(combat_win, 2, 12, "Vous etes empoisonne! -3 PV");
    }
    else if (strcmp(creature->effet_special, "paralysie") == 0) {
        // Logique paralysie (prochain tour sauté)
        afficher_fenetre(combat_win, 2, 12, "Vous etes paralyse!");
    }
    else if (strcmp(creature->effet_special, "saignement") == 0) {
        // Logique saignement
        plongeur->gestion_fatigue_vie->points_de_vie -= 2;
        afficher_fenetre(combat_win, 2, 12, "Vous saignez! -2 PV");
    }
}

void free_combat_plongeur(Combat_plongeur* plongeur_combat){
    if (plongeur_combat != NULL) {        
        free(plongeur_combat);
    }
}
