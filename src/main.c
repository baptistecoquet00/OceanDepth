#include "../include/joueur.h"
#include "../include/utilitaire.h"
#include "../include/interface_combat.h"
#include "../include/systeme_combat.h"

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



CreatureMarine* creer_requin_tigre() {
    CreatureMarine *requin = malloc(sizeof(CreatureMarine));
    
    requin->id = 1;
    strncpy(requin->nom, "Requin-Tigre", sizeof(requin->nom)-1);
    requin->points_de_vie_max = 100;
    requin->points_de_vie_actuels = 100;
    requin->attaque_minimale = 10;
    requin->attaque_maximale = 20;
    requin->defense = 8;
    requin->vitesse = 15;
    strncpy(requin->effet_special, "saignement", sizeof(requin->effet_special)-1);
    requin->est_vivant = 1;
    
    return requin;
}


int main() {
    // Initialisation
    srand(time(NULL));
    
    Arene *jeu_arene = nouvelle_arene(1024 * 1024); // 1MB
    GestionFenetre gf;
    nouvelle_gf(&gf, jeu_arene);
    
    // Création du joueur
    Plongeur *nv_plongeur = nouveau_plongeur();
    Combat_plongeur *nv_plongeur_combat = nouveau_combat_plongeur(nv_plongeur);

    // Création de l'ennemi
    CreatureMarine *ennemi = creer_requin_tigre();
    
    printf("=== LANCEMENT DU COMBAT ===\n");
    printf("Joueur: %d PV, %d O2\n", 
           nv_plongeur_combat->gestion_fatigue_vie->points_de_vie,
           nv_plongeur_combat->gestion_fatigue_vie->niveau_oxygene);
    printf("Ennemi: %s (%d PV)\n", ennemi->nom, ennemi->points_de_vie_actuels);
    
    // Création et exécution du système de combat
    SystemeCombat *combat = creer_systeme_combat(&gf, nv_plongeur_combat, ennemi);
    executer_combat(combat);
    
    // Nettoyage
    printf("Combat terminé. Nettoyage...\n");
    
    // Libération mémoire
    free(ennemi);
    free_combat_plongeur(nv_plongeur_combat);
    free_plongeur(nv_plongeur);

    // Note: Tu devras implémenter detruire_systeme_combat() selon ta gestion mémoire
    detruire_systeme_combat(combat);
    
    arene_detruite(jeu_arene);
    
    printf("=== FIN DU PROGRAMME ===\n");
    return 0;
}