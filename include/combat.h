#ifndef COMBAT_H
#define COMBAT_H

#include "joueur.h"

#define NB_MAXIMUM_ATTAQUE_PAR_TOUR 3
#define NB_MOYEN_ATTAQUE_PAR_TOUR 2
#define NB_BAS_ATTAQUE_PAR_TOUR 1

typedef struct
{
    int nb_attaque_par_tour;// Jusqu'a 3 maximum
    int attaque_normle;
    int attaque_special;
    int competence_special;
    Plongeur fatigue;
} Combat_plongeur;


Combat_plongeur combat_calcul_degats(int attaque_joueurs, int bonus_arme);
Combat_plongeur combat_calcul_fatigue(Combat_plongeur plongeur_combat,int niveau_fatigue);
Combat_plongeur combat_gestion_vie(int points_de_vie);
Combat_plongeur combat_gestion_oxygene(Combat_plongeur plongeur_combat,Plongeur plongeur);
#endif