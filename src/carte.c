#include "../include/carte.h"
#include "../include/systeme_fenetre.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CarteOcean* creer_carte(Arene *arene) {
    CarteOcean *carte = arene_allocation(arene, sizeof(CarteOcean));
    if (!carte) return NULL;
    
    carte->arene = arene;
    carte->pos_x = 0;
    carte->pos_y = 0;
    
    initialiser_zones(carte);
    return carte;
}

void initialiser_zones(CarteOcean *carte) {
    // Ligne 0 - Surface (0m)
    for (int x = 0; x < CARTE_LARGEUR; x++) {
        carte->zones[0][x].profondeur = PROFONDEUR_SURFACE;
        carte->zones[0][x].decouverte = true;
        carte->zones[0][x].accessible = true;
    }
    
    // Configuration des zones spécifiques
    // Surface
    carte->zones[0][0] = (ZoneCarte){TYPE_BASE, "🏝", "Base", "SAUF", 0, true, true, PROFONDEUR_SURFACE};
    carte->zones[0][1] = (ZoneCarte){TYPE_OCEAN, "🌊", "Océan", "", 0, true, true, PROFONDEUR_SURFACE};
    carte->zones[0][2] = (ZoneCarte){TYPE_OCEAN, "🌊", "Océan", "", 0, true, true, PROFONDEUR_SURFACE};
    carte->zones[0][3] = (ZoneCarte){TYPE_BATEAU, "⛵", "Bateau", "SHOP", 0, true, true, PROFONDEUR_SURFACE};
    
    // Zone 1 (50m)
    for (int x = 0; x < CARTE_LARGEUR; x++) {
        carte->zones[1][x].profondeur = PROFONDEUR_ZONE1;
        carte->zones[1][x].decouverte = false;
        carte->zones[1][x].accessible = true;
    }
    carte->zones[1][0] = (ZoneCarte){TYPE_RECIF, "🐠", "Récif", "3 ENM", 3, false, true, PROFONDEUR_ZONE1};
    carte->zones[1][1] = (ZoneCarte){TYPE_EPAVE, "💰", "Épave", "TRÉSOR", 0, false, true, PROFONDEUR_ZONE1};
    carte->zones[1][2] = (ZoneCarte){TYPE_ALGUES, "🌿", "Algues", "5 ENM", 5, false, true, PROFONDEUR_ZONE1};
    carte->zones[1][3] = (ZoneCarte){TYPE_GROTTE, "🕳", "Grotte", "SAUF", 0, false, true, PROFONDEUR_ZONE1};
    
    // Zone 2 (150m)
    for (int x = 0; x < CARTE_LARGEUR; x++) {
        carte->zones[2][x].profondeur = PROFONDEUR_ZONE2;
        carte->zones[2][x].decouverte = false;
        carte->zones[2][x].accessible = false;
    }
    carte->zones[2][0] = (ZoneCarte){TYPE_REQUIN, "🦈", "Requin", "BOSS", 1, false, false, PROFONDEUR_ZONE2};
    carte->zones[2][1] = (ZoneCarte){TYPE_VIDE, "❌", "Vide", "", 0, false, false, PROFONDEUR_ZONE2};
    carte->zones[2][2] = (ZoneCarte){TYPE_KRAKEN, "🐙", "Kraken", "8 ENM", 8, false, false, PROFONDEUR_ZONE2};
    carte->zones[2][3] = (ZoneCarte){TYPE_VIDE, "❌", "Vide", "", 0, false, false, PROFONDEUR_ZONE2};
    
    // Zone 3 (300m)
    for (int x = 0; x < CARTE_LARGEUR; x++) {
        carte->zones[3][x].profondeur = PROFONDEUR_ZONE3;
        carte->zones[3][x].decouverte = false;
        carte->zones[3][x].accessible = false;
        carte->zones[3][x] = (ZoneCarte){TYPE_INCONNU, "❓", "Inconnu", "", 0, false, false, PROFONDEUR_ZONE3};
    }
}

bool deplacement_possible(const CarteOcean *carte, int dx, int dy) {
    int new_x = carte->pos_x + dx;
    int new_y = carte->pos_y + dy;
    
    if (new_x < 0 || new_x >= CARTE_LARGEUR || new_y < 0 || new_y >= CARTE_HAUTEUR) {
        return false;
    }
    
    return carte->zones[new_y][new_x].accessible;
}

void deplacer_joueur(CarteOcean *carte, int dx, int dy) {
    if (deplacement_possible(carte, dx, dy)) {
        carte->pos_x += dx;
        carte->pos_y += dy;
        explorer_zone(carte, carte->pos_x, carte->pos_y);
    }
}

ZoneCarte* get_zone_actuelle(CarteOcean *carte) {
    return &carte->zones[carte->pos_y][carte->pos_x];
}

void explorer_zone(CarteOcean *carte, int x, int y) {
    carte->zones[y][x].decouverte = true;
}

void detruire_carte(CarteOcean *carte) {
    // L'arène gère la libération mémoire
}