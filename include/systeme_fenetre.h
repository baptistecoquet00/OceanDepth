#ifndef SYSTEME_FENETRE_H
#define SYSTEME_FENETRE_H

#include <stddef.h>
#include <stdbool.h>

// #define ARENE_IMPLEMENTATION
#include "gm.h"

#define MAX_FENETRE 10
#define ECRAN_LARGEUR 80
#define ECRAN_HAUTEUR 24

typedef struct {
    int x,y;
    int largeur, hauteur;
    char *tampon;
    char titre[50];
    bool visible;
    int z_index;
} Fenetre;

typedef struct {
    Fenetre fenetres[MAX_FENETRE];
    int nb_fenetre;
    char ecran_tampon[ECRAN_HAUTEUR][ECRAN_LARGEUR];
    Arene* arene;
}GestionFenetre;

void nouvelle_gf(GestionFenetre *gf,Arene *arene);
Fenetre* gf_creer_fenetre(GestionFenetre *gf, int x,int y,int l,int h,const char *titre);
void gf_detruire_fenetre(GestionFenetre *gf,Fenetre *fenetre);
void gf_rendu(GestionFenetre *gf);

void fenetre_nettoyer(Fenetre *fenetre);
void fenetre_dessiner_bordure(Fenetre *fenetre);
void afficher_fenetre(Fenetre *fenetre, int x,int y, const char *texte);
void afficher_fenetref(Fenetre *fenetre,int x, int y , const char *format,...);

void fenetre_dessiner_bordure_debug(Fenetre *fen);
void gf_rendu_debug(GestionFenetre *gf);
#endif