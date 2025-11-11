#include <stdio.h>
#include <stdlib.h>
#include "../include/combat.h"
#include "../include/joueur.h"
#include "../include/utilitaire.h"

#include "../include/interface_combat.h"

// main.c

#include <unistd.h>  // pour sleep()

#define ARENE_IMPLEMENTATION
#include "../include/gm.h"
#include "../include/systeme_fenetre.h"

void afficher_combat_sous_marin() {
    // ✅ CRÉATION DE L'ARÈNE (une seule pour tout le jeu)
    Arene *jeu_arene = nouvelle_arene(1024 * 1024); // 1MB
    
    GestionFenetre gf;
    nouvelle_gf(&gf, jeu_arene);  // Passe ton arène au gestionnaire
    
    // Création des fenêtres (utilise ton arène en interne)
    Fenetre *combat_win = gf_creer_fenetre(&gf, 5, 5, 70, 15, "COMBAT SOUS-MARIN");
    //Fenetre *stats_win = gf_creer_fenetre(&gf, 5, 19, 70, 4, "STATISTIQUES");
    
    if (!combat_win /*|| !stats_win*/) {
        printf("Erreur création fenêtres!\n");
        return;
    }
    
    // Préparer l'interface de combat
    fenetre_dessiner_bordure(combat_win);
    //fenetre_dessiner_bordure(stats_win);
    
    // Dans ton code :
    
    afficher_fenetre(combat_win, 1, 1, "Vous attaquez le Requin-Tigre");
    afficher_fenetre(combat_win, 25, 3, "PLONGEUR VS REQUIN");
    afficher_fenetre(combat_win, 28, 4, "♠        🦈");
    afficher_fenetre(combat_win, 20, 5, "---------> <---------");
    
    
    // Simulation de combat
//     for (int tour = 1; tour <= 3; tour++) {
//     // Efface les anciennes lignes
//     afficher_fenetre(combat_win, 2, 7, "                                    ");
//     afficher_fenetre(combat_win, 2, 8, "                                    ");
//     afficher_fenetre(combat_win, 2, 9, "                                    ");
    
//     // Nouveau contenu sur des lignes DIFFÉRENTES
//     afficher_fenetref(combat_win, 2, 7, "Tour %d", tour);
//     afficher_fenetref(combat_win, 2, 8, "Dégâts: %d points", tour * 5);
//     afficher_fenetref(combat_win, 2, 9, "Riposte: -%d PV", tour * 6);
    
//     gf_rendu(&gf);
//     sleep(2);
// }

// EFFACE tout le contenu de ta boucle et remplace par :
for (int tour = 1; tour <= 3; tour++) {
    // Efface les anciennes lignes de combat
    afficher_fenetre(combat_win, 2, 7, "                                    ");
    afficher_fenetre(combat_win, 2, 8, "                                    "); 
    afficher_fenetre(combat_win, 2, 9, "                                    ");
    afficher_fenetre(combat_win, 2, 10, "                                   ");
    
    // Nouveau contenu
    afficher_fenetref(combat_win, 2, 7, "Tour %d", tour);
    afficher_fenetref(combat_win, 2, 8, "Dégâts infligés: %d points", tour * 5);
    afficher_fenetref(combat_win, 2, 9, "Requin riposte: -%d PV", tour * 6);
    
    gf_rendu(&gf);
    sleep(2);
}


    // ✅ NETTOYAGE FACILE : une seule libération pour tout !
    arene_detruite(jeu_arene);
    // Toutes les fenêtres sont automatiquement libérées avec l'arène
}

int main() {
    CreatureMarine tab[5];
    int nb = 5;   

    generer_creatures(tab, &nb, 100); 

    // for (int i = 0; i < nb; i++) {
    //     afficher_creature(&tab[i]);
    // }

    // Combat_plongeur nv_plongeur_combat = {
    //     .nb_attaque_par_tour = NB_MAXIMUM_ATTAQUE_PAR_TOUR,
    //     .attaque_normale = 15,
    //     .attaque_special = 18,
    //     .competence_special = 8,
    //     .gestion_fatigue_vie = nouveau_plongeur()
    // };

    Plongeur* nv_plongeur = nouveau_plongeur();
    Combat_plongeur* nv_plongeur_combat = nouveau_combat_plongeur(nv_plongeur);
    afficher_plongeur(nv_plongeur_combat->gestion_fatigue_vie);


    nv_plongeur_combat->gestion_fatigue_vie->niveau_fatigue = 5;
    separateur_sections();
    afficher_plongeur(nv_plongeur_combat->gestion_fatigue_vie);
    
    separateur_sections();
    combat_plongeur_calcul_fatigue(nv_plongeur_combat);
    afficher_combat_plongeur(nv_plongeur_combat);

    int degats_calculees = combat_calcul_degats(nv_plongeur_combat->attaque_special,nv_plongeur_combat->competence_special);
    printf("Degats infligés : %d\n",degats_calculees);
    combat_plongeur_calcul_fatigue(nv_plongeur_combat);
    printf("Nombre d'attaque par tour : %d\n",nv_plongeur_combat->nb_attaque_par_tour);
    
    separateur_sections();
    nv_plongeur_combat->gestion_fatigue_vie->niveau_oxygene = 0;
    combat_plongeur_gestion_oxygene(nv_plongeur_combat);
    
    if(nv_plongeur_combat->gestion_fatigue_vie->niveau_oxygene <= OXYGENE_CRITIQUE){
        oxygene_critique(nv_plongeur_combat->gestion_fatigue_vie);
    }

    if(nv_plongeur_combat->gestion_fatigue_vie->points_de_vie <= 0){
        affiche_est_mort();
    }

    afficher_plongeur(nv_plongeur_combat->gestion_fatigue_vie);
    
    separateur_sections();
    printf("Niveau d'oxygène : %d\n",nv_plongeur_combat->gestion_fatigue_vie->niveau_oxygene);

    free_combat_plongeur(nv_plongeur_combat);
    free_plongeur(nv_plongeur);
    afficher_combat_sous_marin();
    return 0;
}