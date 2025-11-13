#ifndef SYSTEME_COMBAT_H
#define SYSTEME_COMBAT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "interface_combat.h"
#include "tour_par_tour.h"
#include "interface_stats_joueur.h"
#include "joueur.h"
#include "combat.h"
#include "creatures.h"
#include "systeme_fenetre.h"
#include "sauvegarde.h"




typedef struct {
    Combat_plongeur *plongeur;
    CreatureMarine *ennemi;
    TourJoueur tour;
    InterfaceCombat *interface;
    GestionFenetre *gf;
    bool combat_actif;
    GestionnaireCompetences *competences;  // ← NOUVELLES COMPÉTENCES
} SystemeCombat;


// Système
SystemeCombat* creer_systeme_combat(GestionFenetre *gf, Combat_plongeur *plongeur, CreatureMarine *ennemi);
void executer_combat(SystemeCombat *systeme);
void executer_tour_joueur(SystemeCombat *systeme);
void executer_tour_ennemi(SystemeCombat *systeme);
bool verifier_fin_combat(SystemeCombat *systeme);
void detruire_systeme_combat(SystemeCombat *systeme);

#endif