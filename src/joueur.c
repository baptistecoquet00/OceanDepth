#include <stdio.h>
#include <stdlib.h>
#include "include/joueur.h"

Plongeur* nouveau_plongeur() {
    Plongeur* plongeur;
    plongeur->points_de_vie = 100;
    plongeur->points_de_vie_max = 100;
    plongeur->niveau_oxygene = 100;
    plongeur->niveau_oxygene_max = 100;
    plongeur->niveau_fatigue = 0;
    plongeur->perles = 0;
    return plongeur;
}

