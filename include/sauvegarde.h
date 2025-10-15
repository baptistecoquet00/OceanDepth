#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

#include "joueur.h"
#include "creatures.h"
#include "inventaire.h"

int save_game(const char *filename, Plongeur *p, Inventaire *inv, int profondeur, CreatureMarine creatures[], int nb);
int load_game(const char *filename, Plongeur *p, Inventaire *inv, int *profondeur, CreatureMarine creatures[], int *nb);

#endif
