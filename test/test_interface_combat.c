#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../include/utilitaire.h"

#define ARENE_IMPLEMENTATION
#include "../include/gm.h"
#include "../include/systeme_fenetre.h"


void test_contenu() {
    Arene *jeu_arene = nouvelle_arene(1024 * 1024);
    GestionFenetre gf;
    nouvelle_gf(&gf, jeu_arene);
    
    Fenetre *test_win = gf_creer_fenetre(&gf, 5, 5, 30, 8, "TEST");
    fenetre_dessiner_bordure(test_win);
    
    // Test différentes positions
    afficher_fenetre(test_win, 1, 1, "L1:Position(1,1)");
    afficher_fenetre(test_win, 1, 2, "L2:Position(1,2)"); 
    afficher_fenetre(test_win, 2, 3, "L3:Position(2,3)");
    afficher_fenetre(test_win, 5, 4, "L4:Position(5,4)");
    
    gf_rendu(&gf);
    getchar();
    
    arene_detruite(jeu_arene);
}

void test_format(){
    Arene *jeu_arene = nouvelle_arene(1024 * 1024); // 1MB
    
    GestionFenetre gf;
    nouvelle_gf(&gf, jeu_arene);  // Passe ton arène au gestionnaire
    Fenetre *combat_win = gf_creer_fenetre(&gf, 5, 5, 70, 15, "COMBAT SOUS-MARIN");
    for (int tour = 1; tour <= 3; tour++) {
        printf("=== TOUR %d ===\n", tour);
        
        // Test 1: Simple (devrait marcher)
        afficher_fenetre(combat_win, 2, 7, "SIMPLE");
        
        // Test 2: Formaté (le problème)
        afficher_fenetref(combat_win, 2, 8, "FORMATE %d", tour);
        
        gf_rendu(&gf);
        getchar();
    }
}

int main(){
    test_contenu();
    test_format();
    return 0;
}