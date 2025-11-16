#ifndef CARTE_H
#define CARTE_H

#include <stdbool.h>
#include "creatures.h"
#include "joueur.h"
#include "gm.h"

#define CARTE_LARGEUR 4
#define CARTE_HAUTEUR 4
#define NOM_ZONE_TAILLE 32
#define DESCRIPTION_TAILLE 64

typedef enum {
    TYPE_BASE,
    TYPE_OCEAN,
    TYPE_BATEAU,
    TYPE_RECIF,
    TYPE_EPAVE,
    TYPE_ALGUES,
    TYPE_GROTTE,
    TYPE_REQUIN,
    TYPE_KRAKEN,
    TYPE_INCONNU,
    TYPE_VIDE
} TypeZone;

typedef enum {
    PROFONDEUR_SURFACE = 0,      // 0m
    PROFONDEUR_ZONE1 = 50,       // 50m
    PROFONDEUR_ZONE2 = 150,      // 150m
    PROFONDEUR_ZONE3 = 300       // 300m
} Profondeur;

typedef struct {
    TypeZone type;
    char emoji[8];
    char nom[NOM_ZONE_TAILLE];
    char description[DESCRIPTION_TAILLE];
    int nb_ennemis;
    bool decouverte;
    bool accessible;
    Profondeur profondeur;
} ZoneCarte;

typedef struct {
    ZoneCarte zones[CARTE_HAUTEUR][CARTE_LARGEUR];
    int pos_x;
    int pos_y;
    Arene *arene;
} CarteOcean;

// Initialisation
CarteOcean* creer_carte(Arene *arene);
void initialiser_zones(CarteOcean *carte);

// Navigation
bool deplacement_possible(const CarteOcean *carte, int dx, int dy);
void deplacer_joueur(CarteOcean *carte, int dx, int dy);

// Affichage
void afficher_carte_complete(const CarteOcean *carte);
void afficher_zone_actuelle(const CarteOcean *carte);

// Interactions
ZoneCarte* get_zone_actuelle(CarteOcean *carte);
void explorer_zone(CarteOcean *carte, int x, int y);

// Gestion
void detruire_carte(CarteOcean *carte);

#endif