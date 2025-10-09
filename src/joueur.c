#include <stdio.h>
#include <stdlib.h>
#include "../include/joueur.h"

Plongeur nouveau_plongeur() {
    //Plongeur* plongeur = (Plongeur*)malloc(sizeof(Plongeur));
    Plongeur plongeur;
    plongeur.points_de_vie = POINT_DE_VIE_MAX;
    plongeur.points_de_vie_max = POINT_DE_VIE_MAX;
    plongeur.niveau_oxygene = OXYGENE_MAX;
    plongeur.niveau_oxygene_max = OXYGENE_MAX;
    plongeur.niveau_fatigue = FATIGUE_NV_ZERO;
    plongeur.perles = 0;
    return plongeur;
}

void afficher_plongeur(Plongeur plongeur){
    printf("Points de vie du plongeur: %d\n",plongeur.points_de_vie);
    printf("Niveau d'oxygene du plongeur: %d\n",plongeur.niveau_oxygene);
    printf("Niveau fatigue du plongeur: %d\n",plongeur.niveau_fatigue);
    printf("Nombre de perles du plongeur: %d\n",plongeur.perles);
}

int etat_perles(Plongeur plongeur){
    int perles = plongeur.perles;
    return perles;
}

int etat_vie(Plongeur plongeur){
    int pv = plongeur.points_de_vie;
    return pv;
}

int est_mort(Plongeur plongeur){
    if(plongeur.points_de_vie <= ZERO_POINT_DE_VIE){
        return 0;
    }
    return -1;
}
void affiche_est_mort(){
    printf("\e[1;31mVOUS ETES MORT\e[0m\n");
}

int vie_maximum(Plongeur plongeur){
    if(plongeur.points_de_vie == plongeur.points_de_vie_max){
        //printf("POINTS DE VIE MAXIMUM\n");
        return 0;
    }
    return -1;
}

int oxygene_vide(Plongeur plongeur){
    if(plongeur.niveau_oxygene <= ZERO_OXYGENE){
        plongeur.niveau_oxygene = ZERO_OXYGENE;
        return plongeur.niveau_oxygene;
    }
    return plongeur.niveau_oxygene;
}

void oxygene_critique(Plongeur plongeur){
    if(plongeur.niveau_oxygene<10){
        printf("\e[1;31m!!!ALERTE ROUGE!!!\e[0m\n");
        return;
    }
    return;
}