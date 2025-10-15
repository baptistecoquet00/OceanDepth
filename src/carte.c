#include <string.h>
#include "carte.h"

void init_map(Carte *c) {
    c->profondeur = 50;
    strcpy(c->zone, "Récif Corallien");
    c->courant = 0;
    c->visibilite = 2;
}
