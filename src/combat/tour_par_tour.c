#include "../../include/tour_par_tour.h"
#include <stdbool.h>

void nouveau_tour(TourJoueur* tour){
    tour->tour_joueur = true;
    tour->action_effectuee = false;
    tour->numero_tour = 1;
}

void tour_commencer_joueur(TourJoueur *tour){
    tour->tour_joueur = true;
    tour->action_effectuee = false;
}

void tour_action_effectuee(TourJoueur *tour){
    tour->action_effectuee =true;
}

void tour_passer_creature(TourJoueur *tour){
    tour->tour_joueur = false;
}

void tour_suivant(TourJoueur *tour){
    tour->numero_tour++;
    tour_commencer_joueur(tour);
}

bool tour_est_au_joueur(const TourJoueur *tour){
    return tour->tour_joueur;
}

bool tour_attente_action(const TourJoueur *tour) {
    return tour->tour_joueur && !tour->action_effectuee;
}

int tour_numero(const TourJoueur *tour) {
    return tour->numero_tour;
}