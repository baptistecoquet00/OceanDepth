#ifndef INTERFACE_COMBAT_H
#define INTERFACE_COMBAT_H
#include "combat.h"
#include "joueur.h"
#include "creatures.h"

#define LARGEUR_INTERFACE 9
#define LONGEUR_INTERFACE 62

typedef struct 
{
    Combat_plongeur plongeur_combat;// champs degats infligés , nb_attaque_tours
    Plongeur plongeur; // Champs fatigue, oxygene, points de vie
    char interface[LARGEUR_INTERFACE][LONGEUR_INTERFACE]; // taille de l'interface
}Interface_combat;

Interface_combat nouvelle_interface_combat();
void afficher_interface_combat_exemple(Combat_plongeur* plongeur_combat);//exemple
void afficher_interface_combat(/*Interface_combat interface,Combat_plongeur plongeur_combat, CreatureMarine creature*/);

#endif