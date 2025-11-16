#include <stdio.h>
#include <stdlib.h>

#include "../../include/interface_stats_joueur.h"


void afficher_stats_joueur(GestionFenetre *gf , Plongeur *joueur) {
    Fenetre *stats_win = gf_creer_fenetre(gf, 5, 5, 90, 10, "STATISTIQUES");
    
    printf("DEBUG: Début afficher_stats_joueur\n");

    if(!stats_win) {
        printf("Erreur création fenêtre stats!\n");
        return;
    }

     printf("DEBUG: Fenêtre créée à (%d,%d) taille %dx%d\n", 
           stats_win->x, stats_win->y, stats_win->largeur, stats_win->hauteur);

    fenetre_dessiner_bordure(stats_win);
    
    // Calcul des pourcentages
    float pourcentage_vie = (float)joueur->points_de_vie / joueur->points_de_vie_max;
    float pourcentage_oxygene = (float)joueur->niveau_oxygene / joueur->niveau_oxygene_max;
    float pourcentage_fatigue = (float)joueur->niveau_fatigue / 5.0f;
    
    // Création des barres visuelles
    char barre_vie[50], barre_oxygene[50], barre_fatigue[10];
    
    // Barre de vie [xxxxxxxxxx.....]
    creer_barre_visuelle(barre_vie, sizeof(barre_vie), pourcentage_vie, 30);
    afficher_fenetref(stats_win, 2, 1, "Vie %s %d/%d", barre_vie, joueur->points_de_vie, joueur->points_de_vie_max);
    
    // Barre d'oxygène
    creer_barre_visuelle(barre_oxygene, sizeof(barre_oxygene), pourcentage_oxygene, 30);
    afficher_fenetref(stats_win, 2, 2, "Oxygene %s %d/%d", barre_oxygene, joueur->niveau_oxygene, joueur->niveau_oxygene_max);
    
    // Barre de fatigue (plus courte)
    creer_barre_visuelle(barre_fatigue, sizeof(barre_fatigue), pourcentage_fatigue, 10);
    afficher_fenetref(stats_win, 55, 1, "Fatigue %s %d/5", barre_fatigue, joueur->niveau_fatigue);
    
    // Perles
    afficher_fenetref(stats_win, 55, 2, "Perles: %d", joueur->perles);
}

void mettre_a_jour_stats_joueur(Fenetre *stats_win, Plongeur *joueur) {
    if (!stats_win || !stats_win->tampon) return;
    
    // EFFACER l'ancien contenu (garder la bordure)
    for (int y = 1; y < stats_win->hauteur - 1; y++) {
        for (int x = 1; x < stats_win->largeur - 1; x++) {
            stats_win->tampon[y * stats_win->largeur + x] = ' ';
        }
    }
    
    // Calcul des pourcentages
    float pourcentage_vie = (float)joueur->points_de_vie / joueur->points_de_vie_max;
    float pourcentage_oxygene = (float)joueur->niveau_oxygene / joueur->niveau_oxygene_max;
    float pourcentage_fatigue = (float)joueur->niveau_fatigue / 5.0f;
    
    // Création des barres visuelles
    char barre_vie[50], barre_oxygene[50], barre_fatigue[50];
    
    creer_barre_visuelle(barre_vie, sizeof(barre_vie), pourcentage_vie, 25);
    creer_barre_visuelle(barre_oxygene, sizeof(barre_oxygene), pourcentage_oxygene, 25);
    creer_barre_visuelle(barre_fatigue, sizeof(barre_fatigue), pourcentage_fatigue, 10);
    
    // Afficher les nouvelles stats
    afficher_fenetref(stats_win, 2, 1, "Vie %s %d/%d", barre_vie, joueur->points_de_vie, joueur->points_de_vie_max);
    afficher_fenetref(stats_win, 2, 2, "Oxygene %s %d/%d", barre_oxygene, joueur->niveau_oxygene, joueur->niveau_oxygene_max);
    afficher_fenetref(stats_win, 50, 1, "Fatigue %s %d/5", barre_fatigue, joueur->niveau_fatigue);
    afficher_fenetref(stats_win, 50, 2, "Perles: %d", joueur->perles);
}



void creer_barre_visuelle(char *buffer, size_t taille_buffer, float pourcentage, int longueur_totale) {
    int caracteres_remplis = (int)(pourcentage * longueur_totale);
    int caracteres_vides = longueur_totale - caracteres_remplis;
    
    // S'assurer qu'on ne dépasse pas le buffer
    caracteres_remplis = (caracteres_remplis > longueur_totale) ? longueur_totale : caracteres_remplis;
    caracteres_vides = (caracteres_vides < 0) ? 0 : caracteres_vides;
    
    // Construire la barre
    int pos = 0;
    buffer[pos++] = '[';
    
    for (int i = 0; i < caracteres_remplis; i++) {
        if (pos < taille_buffer - 1) buffer[pos++] = 'x';
    }
    
    for (int i = 0; i < caracteres_vides; i++) {
        if (pos < taille_buffer - 1) buffer[pos++] = '.';
    }
    
    if (pos < taille_buffer - 1) buffer[pos++] = ']';
    buffer[pos] = '\0';
}
