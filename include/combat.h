#ifndef COMBAT_H
#define COMBAT_H
#include "creatures.h"
#include "joueur.h"

#define NB_MAXIMUM_ATTAQUE_PAR_TOUR 3
#define NB_MOYEN_ATTAQUE_PAR_TOUR 2
#define NB_BAS_ATTAQUE_PAR_TOUR 1

typedef struct
{
    int nb_attaque_par_tour;// Jusqu'a 3 maximum
    int attaque_normale;
    int attaque_special;
    int competence_special;
    Plongeur* gestion_fatigue_vie;
} Combat_plongeur;

Combat_plongeur* nouveau_combat_plongeur(Plongeur* plongeur);
void afficher_combat_plongeur(Combat_plongeur* combat_plongeur);
int combat_calcul_degats(int attaque_joueurs, int bonus_arme);
void combat_calcul_fatigue(Combat_plongeur* plongeur_combat);
void combat_gestion_vie(Combat_plongeur* plongeur_combat,CreatureMarine* creature);
void combat_gestion_oxygene(Combat_plongeur* plongeur_combat);
#endif