#include "../include/joueur.h"
#include "../include/utilitaire.h"
#include "../include/interface_combat.h"


// main.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // pour sleep()
#include <time.h>

#define ARENE_IMPLEMENTATION
#include "../include/gm.h"
#include "../include/systeme_fenetre.h"
#include "../include/interface_stats_joueur.h"
#include "../include/combat.h"


void afficher_combat_sous_marin(GestionFenetre *gf) {
    // Création des fenêtres (utilise ton arène en interne)
    Fenetre *combat_win = gf_creer_fenetre(gf, 5, 5, 70, 15, "COMBAT SOUS-MARIN");
    
    if (!combat_win) {
        printf("Erreur création fenêtres!\n");
        return;
    }
    
    // Préparer l'interface de combat
    fenetre_dessiner_bordure(combat_win);
 
    // Contenu fixe
    afficher_fenetre(combat_win, 1, 1, "Vous attaquez le Requin-Tigre");
    afficher_fenetre(combat_win, 25, 3, "PLONGEUR VS REQUIN");
    afficher_fenetre(combat_win, 28, 4, "♠        🦈");
    afficher_fenetre(combat_win, 20, 5, "---------> <---------");
    
    // Boucle de combat
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
        
        gf_rendu(gf);
        sleep(2);
    }
}



int main() {
    // CreatureMarine tab[5];
    // int nb = 5;   

    // generer_creatures(tab, &nb, 100); 
    // Combat_plongeur* nv_plongeur_combat = nouveau_combat_plongeur(nv_plongeur);
    // afficher_plongeur(nv_plongeur_combat->gestion_fatigue_vie);


    // nv_plongeur_combat->gestion_fatigue_vie->niveau_fatigue = 5;
    // separateur_sections();
    // afficher_plongeur(nv_plongeur_combat->gestion_fatigue_vie);
    
    // separateur_sections();
    // combat_plongeur_calcul_fatigue(nv_plongeur_combat);
    // afficher_combat_plongeur(nv_plongeur_combat);

    // int degats_calculees = combat_calcul_degats(nv_plongeur_combat->attaque_special,nv_plongeur_combat->competence_special);
    // printf("Degats infligés : %d\n",degats_calculees);
    // combat_plongeur_calcul_fatigue(nv_plongeur_combat);
    // printf("Nombre d'attaque par tour : %d\n",nv_plongeur_combat->nb_attaque_par_tour);
    
    // separateur_sections();
    // nv_plongeur_combat->gestion_fatigue_vie->niveau_oxygene = 0;
    // combat_plongeur_gestion_oxygene(nv_plongeur_combat);
    
    // if(nv_plongeur_combat->gestion_fatigue_vie->niveau_oxygene <= OXYGENE_CRITIQUE){
    //     oxygene_critique(nv_plongeur_combat->gestion_fatigue_vie);
    // }

    // if(nv_plongeur_combat->gestion_fatigue_vie->points_de_vie <= 0){
    //     affiche_est_mort();
    // }

    // afficher_plongeur(nv_plongeur_combat->gestion_fatigue_vie);
    
    // separateur_sections();
    // printf("Niveau d'oxygène : %d\n",nv_plongeur_combat->gestion_fatigue_vie->niveau_oxygene);

    // free_combat_plongeur(nv_plongeur_combat);
    // free_plongeur(nv_plongeur);

    // Initialisation
    srand(time(NULL));  // Pour l'aléatoire des dégâts
    
    Arene *jeu_arene = nouvelle_arene(1024 * 1024);
    GestionFenetre gf;
    nouvelle_gf(&gf, jeu_arene);
    
    Plongeur *nv_plongeur = nouveau_plongeur();
    Combat_plongeur *nv_plongeur_combat = nouveau_combat_plongeur(nv_plongeur);
    
    // Initialisation du joueur
    nv_plongeur_combat->gestion_fatigue_vie->points_de_vie = 100;
    nv_plongeur_combat->gestion_fatigue_vie->niveau_oxygene = 100;
    
    // Lancement du combat
    Systeme_combat(nv_plongeur_combat, &gf/*, jeu_arene*/);
    
    free_combat_plongeur(nv_plongeur_combat);
    free_plongeur(nv_plongeur);
    arene_detruite(jeu_arene);
    return 0;
}