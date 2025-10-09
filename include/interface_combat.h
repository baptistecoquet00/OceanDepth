#ifndef INTERFACE_COMBAT_H
#define INTERFACE_COMBAT_H
#include "combat.h"
#include "joueur.h"
#include "creatures.h"

typedef struct 
{
    // Un champ qui permet de voir le nombre de dégats infligées
    // un champ permet de voir le nombre de dégats subits
    // un champ qui permet de voir la fatigue
    //un champ qui permet de voir l'oxygene
    char interface[1024][1024];
}Interface_combat;

Interface_combat nouvelle_interface_combat();
void afficher_interface_combat(Interface_combat interface,Combat_plongeur plongeur_combat, CreatureMarine creature);

#endif