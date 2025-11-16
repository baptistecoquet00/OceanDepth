#ifndef INTERFACE_STATS_JOUEUR_H
#define INTERFACE_STATS_JOUEUR_H

#include "joueur.h"
#include "creatures.h"
#include "combat.h"
#include "systeme_fenetre.h"

void afficher_stats_joueur(GestionFenetre *gf, Plongeur *joueur);
void mettre_a_jour_stats_joueur(Fenetre *stats_win, Plongeur *joueur);
void creer_barre_visuelle(char *buffer, size_t taille_buffer, float pourcentage, int longueur_totale);
void afficher_stats_joueur_dans_fenetre(Fenetre *stats_win, Plongeur *joueur);
#endif