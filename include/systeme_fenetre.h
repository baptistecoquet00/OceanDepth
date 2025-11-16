#ifndef SYSTEME_FENETRE_H
#define SYSTEME_FENETRE_H

#include <stddef.h>
#include <stdbool.h>

// #define ARENE_IMPLEMENTATION
#include "gm.h"

#define MAX_FENETRE 10
#define ECRAN_LARGEUR 80
#define ECRAN_HAUTEUR 24

// Codes de couleurs ANSI
#define COULEUR_NOIR     0
#define COULEUR_ROUGE    1
#define COULEUR_VERT     2
#define COULEUR_JAUNE    3
#define COULEUR_BLEU     4
#define COULEUR_MAGENTA  5
#define COULEUR_CYAN     6
#define COULEUR_BLANC    7

// Attributs
#define ATTRIBUT_NORMAL      0
#define ATTRIBUT_GRAS        1
#define ATTRIBUT_SOULIGNE    4
#define ATTRIBUT_CLIGNOTANT  5
#define ATTRIBUT_INVERSER    7

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
} GestionFenetre;

void nouvelle_gf(GestionFenetre *gf,Arene *arene);
Fenetre* gf_creer_fenetre(GestionFenetre *gf, int x,int y,int l,int h,const char *titre);
void gf_detruire_fenetre(GestionFenetre *gf,Fenetre *fenetre);
void gf_rendu(GestionFenetre *gf);

void fenetre_nettoyer(Fenetre *fenetre);
void fenetre_dessiner_bordure(Fenetre *fenetre);
void afficher_fenetre(Fenetre *fenetre, int x,int y, const char *texte);
void afficher_fenetref(Fenetre *fenetre,int x, int y , const char *format,...);

// Nouvelles fonctions à ajouter
void deplacer_curseur_fenetre(Fenetre *fenetre, int y, int x);
void afficher_texte_fenetre(Fenetre *fenetre, const char *texte);
void debut_attribut(Fenetre *fenetre, int attribut);
void fin_attribut(Fenetre *fenetre, int attribut);

void fenetre_dessiner_bordure_debug(Fenetre *fen);
void gf_rendu_debug(GestionFenetre *gf);
#endif