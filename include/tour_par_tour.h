#ifndef TOUR_PAR_TOUR_H
#define TOUR_PAR_TOUR_H

#include <stdbool.h>

typedef struct {
    bool tour_joueur;
    bool action_effectuee;
    int numero_tour;
} TourJoueur;

void nouveau_tour(TourJoueur *tour);
void tour_commencer_joueur(TourJoueur *tour);
void tour_action_effectuee(TourJoueur *tour);

void tour_passer_creature(TourJoueur *tour);
void tour_suivant(TourJoueur *tour);

bool tour_est_au_joueur(const TourJoueur *tour);
bool tour_attente_action(const TourJoueur *tour);
int tour_numero(const TourJoueur *tour);

#endif