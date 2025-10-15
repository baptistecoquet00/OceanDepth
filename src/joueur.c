#include <string.h>
#include "joueur.h"

void init_plongeur(Plongeur *p) {
    p->points_de_vie_max = 100;
    p->points_de_vie = 100;
    p->niveau_oxygene_max = 100;
    p->niveau_oxygene = 100;
    p->niveau_fatigue = 0;
    p->perles = 0;
    p->defense = 5;
    p->attaques_restantes = 3;
    p->paralyse = 0;
    strcpy(p->harpon, "Harpon Rouille");
    strcpy(p->combinaison, "Neoprene Basic");
}
