#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include "../include/combat.h"
#include "../include/utilitaire.h"
#include "../include/joueur.h"

#define ARENE_IMPLEMENTATION
#include "../include/gm.h"
#include "../include/interface_stats_joueur.h"

void test_interface_stats(){
    Arene *jeu_arene = nouvelle_arene(1024 * 1024);
    GestionFenetre gf;
    nouvelle_gf(&gf, jeu_arene);

    Plongeur *nv_plongeur = nouveau_plongeur();
    Combat_plongeur *nv_plongeur_combat = nouveau_combat_plongeur(nv_plongeur);

    // Test spécifique
    nv_plongeur_combat->gestion_fatigue_vie->points_de_vie = 100;
    nv_plongeur_combat->gestion_fatigue_vie->niveau_oxygene = 0;  // Oxygène à 0
    nv_plongeur_combat->gestion_fatigue_vie->niveau_fatigue = 5;
    nv_plongeur_combat->gestion_fatigue_vie->perles = 105;

    Fenetre *stats_win = gf_creer_fenetre(&gf, 5, 5, 90, 6, "STATISTIQUES");
    fenetre_dessiner_bordure(stats_win);

    printf("=== TEST: Perte de vie progressive (O2=0) ===\n");

    int tour = 0;
    while(nv_plongeur_combat->gestion_fatigue_vie->points_de_vie > 0) {
        tour++;
        printf("Tour %d - PV: %d/100\n", tour, nv_plongeur_combat->gestion_fatigue_vie->points_de_vie);
        
        // Mise à jour affichage
        mettre_a_jour_stats_joueur(stats_win, nv_plongeur_combat->gestion_fatigue_vie);
        gf_rendu(&gf);
        oxygene_critique(nv_plongeur);
        //  Appel UNIQUE qui fait -5 PV seulement 
        combat_plongeur_gestion_oxygene(nv_plongeur_combat);
        
        if(nv_plongeur_combat->gestion_fatigue_vie->points_de_vie <= 0) { 
            mettre_a_jour_stats_joueur(stats_win, nv_plongeur_combat->gestion_fatigue_vie);
            gf_rendu(&gf);
            affiche_est_mort();
            printf(">>> MORT au tour %d\n", tour);
            break;
        }
        
        sleep(1);  // Voir la descente progressive
    }

    arene_detruite(jeu_arene);
}

int main(){
    test_interface_stats();
    return 0;
}