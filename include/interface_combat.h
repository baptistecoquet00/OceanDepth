#ifndef INTERFACE_COMBAT_H
#define INTERFACE_COMBAT_H
#include "combat.h"
#include "joueur.h"
#include "creatures.h"

typedef struct 
{
    Combat_plongeur plongeur_combat;// champs degats infligés , nb_attaque_tours
    Plongeur plongeur; // Champs fatigue, oxygene, points de vie
    char interface[9][62]; // taille de l'interface
}Interface_combat;

Interface_combat nouvelle_interface_combat();
void afficher_interface_combat_exemple();
void afficher_interface_combat(/*Interface_combat interface,Combat_plongeur plongeur_combat, CreatureMarine creature*/);

#endif