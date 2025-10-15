#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "creatures.h"

/* On se base sur la profondeur pour ajuster nb et force */

static void init_kraken(CreatureMarine *c, int id) {
    c->id = id;
    strcpy(c->nom, "Kraken");
    c->points_de_vie_max = 140;
    c->points_de_vie_actuels = 120;
    c->attaque_minimale = 25;
    c->attaque_maximale = 35;
    c->defense = 10;
    c->vitesse = 4;
    strcpy(c->effet_special, "etreeinte");
    c->est_vivant = 1;
}

static void init_requin(CreatureMarine *c, int id) {
    c->id = id;
    strcpy(c->nom, "Requin");
    c->points_de_vie_max = 80;
    c->points_de_vie_actuels = 80;
    c->attaque_minimale = 15;
    c->attaque_maximale = 25;
    c->defense = 5;
    c->vitesse = 7;
    strcpy(c->effet_special, "frenesie");
    c->est_vivant = 1;
}

static void init_meduse(CreatureMarine *c, int id) {
    c->id = id;
    strcpy(c->nom, "Méduse");
    c->points_de_vie_max = 30;
    c->points_de_vie_actuels = 30;
    c->attaque_minimale = 8;
    c->attaque_maximale = 15;
    c->defense = 2;
    c->vitesse = 9;
    strcpy(c->effet_special, "paralysie");
    c->est_vivant = 1;
}

static void init_poisson_ep(CreatureMarine *c, int id) {
    c->id = id;
    strcpy(c->nom, "Poisson-Épée");
    c->points_de_vie_max = 80;
    c->points_de_vie_actuels = 80;
    c->attaque_minimale = 18;
    c->attaque_maximale = 28;
    c->defense = 4;
    c->vitesse = 6;
    strcpy(c->effet_special, "perforant");
    c->est_vivant = 1;
}

static void init_crabe(CreatureMarine *c, int id) {
    c->id = id;
    strcpy(c->nom, "Crabe Géant");
    c->points_de_vie_max = 100;
    c->points_de_vie_actuels = 100;
    c->attaque_minimale = 12;
    c->attaque_maximale = 20;
    c->defense = 12;
    c->vitesse = 3;
    strcpy(c->effet_special, "carapace");
    c->est_vivant = 1;
}

void init_creatures_random(CreatureMarine arr[], int *nb, int profondeur) {
    // profondeur influe sur le nombre (1..4 par spec)
    int max = 1 + (profondeur / 200); // simple : 0-199 =>1, 200-399 =>2...
    if (max < 1) max = 1;
    if (max > 4) max = 4;
    int count = 1 + rand() % max; // 1..max
    *nb = count;
    for (int i = 0; i < count; i++) {
        int r = rand() % 5;
        switch (r) {
            case 0: init_kraken(&arr[i], i+1); break;
            case 1: init_requin(&arr[i], i+1); break;
            case 2: init_meduse(&arr[i], i+1); break;
            case 3: init_poisson_ep(&arr[i], i+1); break;
            default: init_crabe(&arr[i], i+1); break;
        }
        // ajustement selon profondeur
        int bonus = profondeur / 100; // +1 par 100m
        arr[i].points_de_vie_max += bonus * 5;
        arr[i].points_de_vie_actuels = arr[i].points_de_vie_max;
        arr[i].attaque_minimale += bonus;
        arr[i].attaque_maximale += bonus;
    }
}
