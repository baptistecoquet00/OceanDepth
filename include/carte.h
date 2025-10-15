#ifndef CARTE_H
#define CARTE_H

typedef struct {
    int profondeur; // en mètres
    char zone[32];
    int courant; // 0 faible, 1 moyen, 2 fort
    int visibilite; // 0 mauvaise, 1 moyenne, 2 bonne
} Carte;

void init_map(Carte *c);

#endif
