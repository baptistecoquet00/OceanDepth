#ifndef JOUEUR_H
#define JOUEUR_H

typedef struct {
    int points_de_vie;
    int points_de_vie_max;
    int niveau_oxygene;
    int niveau_oxygene_max;
    int niveau_fatigue; // 0 à 5
    int perles; // monnaie du jeu
} Plongeur;

typedef enum {
    FATIGUE_NV_ZERO = 0,
    FATIGUE_NV_UN = 1,
    FATIGUE_NV_DEUX = 2,
    FATIGUE_NV_TROIS = 3,
    FATIGUE_NV_QUATRE = 4,
    FATIGUE_NV_CINQ = 5,
}Fatigue;

Plongeur* nouveau_plongeur();
int etat_perles(Plongeur plongeur);
int etat_vie(Plongeur plongeur);
void est_mort(Plongeur plongeur);
void vie_maximum(Plongeur plongeur);

#endif