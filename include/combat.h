#ifndef COMBAT_H
#define COMBAT_H

#include "creatures.h"
#include "joueur.h"
#include "systeme_fenetre.h"

#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define NB_MAXIMUM_ATTAQUE_PAR_TOUR 3
#define NB_MOYEN_ATTAQUE_PAR_TOUR 2
#define NB_BAS_ATTAQUE_PAR_TOUR 1

typedef struct
{
    int nb_attaque_par_tour;// Jusqu'a 3 maximum
    int attaque_normale;
    int attaque_special;
    int competence_special;
    int compteur_attaques;
    Plongeur* gestion_fatigue_vie;
    bool a_fui;
} Combat_plongeur;

Combat_plongeur* nouveau_combat_plongeur(Plongeur* plongeur);
void afficher_combat_plongeur(Combat_plongeur* combat_plongeur);
int combat_calcul_degats(int attaque_joueurs, int bonus_arme);
void combat_plongeur_calcul_fatigue(Combat_plongeur* plongeur_combat, int nombre_attaques_ce_tour);
void combat_plongeur_gestion_vie(Combat_plongeur* plongeur_combat,CreatureMarine* creature);
void combat_plongeur_gestion_oxygene(Combat_plongeur* plongeur_combat);
void Systeme_combat(Combat_plongeur *plongeur_combat, GestionFenetre *gf /*,Arene *jeu_arene*/);
int calculer_degats_creature(CreatureMarine *creature);
void appliquer_effet_special(CreatureMarine *creature, Combat_plongeur *plongeur,Fenetre *combat_win);
void executer_action_joueur(Combat_plongeur *plongeur, CreatureMarine *ennemi, int action,Fenetre *actions_win);
void free_combat_plongeur(Combat_plongeur* plongeur_combat);
#endif