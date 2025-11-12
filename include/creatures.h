#ifndef CREATURES_H
#define CREATURES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CREATURES 8

typedef struct {
    int id;
    char nom[32];
    int points_de_vie_max;
    int points_de_vie_actuels;
    int attaque_minimale;
    int attaque_maximale;
    int defense;
    int vitesse;
    char effet_special[20]; // "paralysie", "poison", "aucun"
    int est_vivant;
} CreatureMarine;

void init_creatures_random(CreatureMarine arr[], int *nb, int profondeur);

#endif