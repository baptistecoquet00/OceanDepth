#include <stdio.h>
#include <stdlib.h>
#include "../include/joueur.h"

Plongeur* nouveau_plongeur() {
    Plongeur* plongeur = (Plongeur*)malloc(sizeof(Plongeur));
    plongeur->points_de_vie = 100;
    plongeur->points_de_vie_max = 100;
    plongeur->niveau_oxygene = 100;
    plongeur->niveau_oxygene_max = 100;
    plongeur->niveau_fatigue = 0;
    plongeur->perles = 0;
    return plongeur;
}

int etat_perles(Plongeur plongeur){
    int perles = plongeur.perles;
    return perles;
}

int etat_vie(Plongeur plongeur){
    int pv = plongeur.points_de_vie;
    return pv;
}

void est_mort(Plongeur plongeur){
    if(plongeur.points_de_vie == 0){
        printf("VOUS ETES MORT !!!!!\n");
        return;
    }
}

void vie_maximum(Plongeur plongeur){
    if(plongeur.points_de_vie == plongeur.points_de_vie_max){
        printf("POINTS DE VIE MAXIMUM\n");
        return;
    }
}
