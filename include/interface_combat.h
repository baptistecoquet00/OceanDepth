#ifndef INTERFACE_COMBAT_H
#define INTERFACE_COMBAT_H
#include "combat.h"
#include "joueur.h"
#include "creatures.h"

typedef struct 
{
    char interface[1024][1024];
}Interface_combat;

Interface_combat nouvelle_interface_combat();
void afficher_interface_combat(Combat_plongeur plongeur_combat, CreatureMarine creature);

#endif