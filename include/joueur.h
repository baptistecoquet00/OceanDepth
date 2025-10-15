#ifndef JOUEUR_H
#define JOUEUR_H

#define MAX_INVENTAIRE 8

typedef struct {
    int points_de_vie;
    int points_de_vie_max;
    int niveau_oxygene;
    int niveau_oxygene_max;
    int niveau_fatigue; // 0..5
    int perles;
    int defense;
    int attaques_restantes; // calculé par fatigue
    int paralyse; // 0 ou nombre de tours
    // équipement simple
    char harpon[32];
    char combinaison[32];
} Plongeur;

void init_plongeur(Plongeur *p);

#endif
