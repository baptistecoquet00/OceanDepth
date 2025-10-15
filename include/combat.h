#ifndef COMBAT_H
#define COMBAT_H

#include "joueur.h"
#include "creatures.h"

void attaque_creatures(Plongeur *joueur, CreatureMarine creatures[], int nb_creatures);
int calcul_attacks_from_fatigue(int fatigue);

#endif
