#ifndef CREATURES_H
#define CREATURES_H

typedef enum {
    KRAKEN,
    REQUIN,
    MEDUSE,
    POISSON_EPEE,
    SERPENT_DE_MER
} TypeCreature;

typedef struct {
    int id; // identifiant unique pour cibler
    char nom[30];
    int points_de_vie_max;
    int points_de_vie_actuels;
    int attaque_minimale;
    int attaque_maximale;
    int defense;
    int vitesse;
    char effet_special[20]; // "paralysie", "poison", "aucun"
    int est_vivant;
} CreatureMarine;

void generer_creatures(CreatureMarine *tableau, int *nb_creatures, int profondeur);
void afficher_creature(const CreatureMarine *creature);

#endif