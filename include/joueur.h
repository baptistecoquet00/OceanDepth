#ifndef JOUEUR_H
#define JOUEUR_H

#define POINT_DE_VIE_MAX 100
#define ZERO_POINT_DE_VIE 0
#define OXYGENE_MAX 100
#define ZERO_OXYGENE 0

typedef struct {
    int points_de_vie;
    int points_de_vie_max;
    int niveau_oxygene;
    int niveau_oxygene_max;
    int niveau_fatigue; // 0 à 5
    int perles; // monnaie du jeu
} Plongeur;

typedef enum {
    FATIGUE_NV_ZERO = 0,
    FATIGUE_NV_UN = 1,
    FATIGUE_NV_DEUX = 2,
    FATIGUE_NV_TROIS = 3,
    FATIGUE_NV_QUATRE = 4,
    FATIGUE_NV_CINQ = 5,
}NV_Fatigue;

Plongeur nouveau_plongeur();
void afficher_plongeur(Plongeur plongeur);
int etat_perles(Plongeur plongeur);
int etat_vie(Plongeur plongeur);
int est_mort(Plongeur plongeur);
void affiche_est_mort();
int vie_maximum(Plongeur plongeur);
int oxygene_vide(Plongeur plongeur);
void oxygene_critique(Plongeur plongeur);

#endif