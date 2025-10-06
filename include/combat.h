#ifndef COMBAT_H
#define COMBAT_H

#include "joueur.h"

typedef struct
{
    int nombre_attaque_par_tour;
} combat;


Plongeur combat_calcul_degats(int attaque_joueurs);
Plongeur combat_calcul_fatigue(Plongeur* plongeur,int niveau_fatigue);
Plongeur combat_gestion_vie(int points_de_vie);

#endif