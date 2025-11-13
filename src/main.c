#include "../include/joueur.h"
#include "../include/utilitaire.h"
#include "../include/interface_combat.h"
#include "../include/systeme_combat.h"
#include "../include/sauvegarde.h"

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


// int main() {
//     // Initialisation
//     srand(time(NULL));
    
//     Arene *jeu_arene = nouvelle_arene(1024 * 1024); // 1MB
//     GestionFenetre gf;
//     nouvelle_gf(&gf, jeu_arene);
    
//     // Création du joueur
//     Plongeur *nv_plongeur = nouveau_plongeur();
//     Combat_plongeur *nv_plongeur_combat = nouveau_combat_plongeur(nv_plongeur);

//     // Création de l'ennemi
//     //CreatureMarine *ennemi = creer_requin_tigre();
//     CreatureMarine creatures[MAX_CREATURES];
//     int nb_creatures;
//     int profondeur = 150;  // Exemple: 150m de profondeur
    
//     init_creatures_random(creatures, &nb_creatures, profondeur);
    
//     printf("=== RENCONTRE ALÉATOIRE À %dm ===\n", profondeur);
//     printf("%d créature(s) générée(s):\n", nb_creatures);
//     for (int i = 0; i < nb_creatures; i++) {
//         printf("- %s (%d PV, Attaque: %d-%d, Defense: %d)\n",
//                creatures[i].nom, 
//                creatures[i].points_de_vie_actuels,
//                creatures[i].attaque_minimale,
//                creatures[i].attaque_maximale,
//                creatures[i].defense);
//     }

//     // printf("=== LANCEMENT DU COMBAT ===\n");
//     // printf("Joueur: %d PV, %d O2\n", 
//     //        nv_plongeur_combat->gestion_fatigue_vie->points_de_vie,
//     //        nv_plongeur_combat->gestion_fatigue_vie->niveau_oxygene);
//     // printf("Ennemi: %s (%d PV)\n", ennemi->nom, ennemi->points_de_vie_actuels);
    
//     CreatureMarine *ennemi = &creatures[0];
    
//     printf("\n=== COMBAT CONTRE %s ===\n", ennemi->nom);

//     // Création et exécution du système de combat
//     SystemeCombat *combat = creer_systeme_combat(&gf, nv_plongeur_combat, ennemi);
//     executer_combat(combat);
    
//     // Nettoyage
//     printf("Combat terminé. Nettoyage...\n");
    
//     // Libération mémoire
//     //free(ennemi);
//     free_combat_plongeur(nv_plongeur_combat);
//     free_plongeur(nv_plongeur);

//     // Note: Tu devras implémenter detruire_systeme_combat() selon ta gestion mémoire
//     detruire_systeme_combat(combat);
    
//     arene_detruite(jeu_arene);
    
//     printf("=== FIN DU PROGRAMME ===\n");
//     return 0;
// }

void debug_creature(const char* nom, CreatureMarine *c) {
    printf("DEBUG %s:\n", nom);
    printf("  Nom: '%s'\n", c->nom);
    printf("  PV: %d/%d\n", c->points_de_vie_actuels, c->points_de_vie_max);
    printf("  Attaque: %d-%d\n", c->attaque_minimale, c->attaque_maximale);
    printf("  Defense: %d\n", c->defense);
    printf("  Vitesse: %d\n", c->vitesse);
    printf("  Effet: '%s'\n", c->effet_special);
    printf("  Vivant: %d\n", c->est_vivant);
    printf("  ID: %d\n", c->id);
    printf("  Taille structure: %zu bytes\n", sizeof(*c));
}

int main() {
    // Charger la sauvegarde directement dans tes structures
    SauvegardeJeu *sauvegarde = charger_sauvegarde_complete("/mnt/c/Users/bcoqu/Documents/GROUPE-15/saves/oceandepths_save_v1.json");
    if (!sauvegarde) {
        printf("Création d'une nouvelle partie...\n");
        return 1;
    }

    
    // Initialiser l'arène et l'interface
    Arene *jeu_arene = nouvelle_arene(1024 * 1024);
    GestionFenetre gf;
    nouvelle_gf(&gf, jeu_arene);
    
    for (int i = 0; i < sauvegarde->nb_creatures; i++) {
        if (sauvegarde->creatures[i].est_vivant) {
            printf("\n--- COMBAT contre %s ---\n", sauvegarde->creatures[i].nom);
            
            SystemeCombat *combat = creer_systeme_combat(&gf, sauvegarde->combat_plongeur, &sauvegarde->creatures[i]);
            executer_combat(combat);
            
            free(combat->interface);
            free(combat);

            // Vérifier si le joueur est mort
            if (sauvegarde->plongeur->points_de_vie <= 0) {
                break;
            }
        }
    }
    
    // Nettoyage
    liberer_sauvegarde(sauvegarde);
    arene_detruite(jeu_arene);
    return 0;
}