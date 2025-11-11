#ifndef INTERFACE_COMBAT_H
#define INTERFACE_COMBAT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "joueur.h"
#include "creatures.h"
#include "combat.h"
#include "interface_combat.h"
#include "interface_stats_joueur.h"

typedef struct {
    Fenetre *combat_win;
    Fenetre *actions_win;
    Fenetre *stats_win;
} InterfaceCombat;

// Initialisation
InterfaceCombat* creer_interface_combat(GestionFenetre *gf);

// Mise à jour
void mettre_a_jour_interface_combat(InterfaceCombat *interface, 
                                   Combat_plongeur *plongeur, 
                                   CreatureMarine *ennemi);

// Affichage des actions disponibles
void afficher_actions_disponibles(InterfaceCombat *interface);

// Affichage des messages de combat
void afficher_message_combat(InterfaceCombat *interface, const char *format, ...);

// Nettoyage
void detruire_interface_combat(InterfaceCombat *interface);

#endif