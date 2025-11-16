#include "../include/joueur.h"
#include "../include/utilitaire.h"
#include "../include/interface_combat.h"
#include "../include/systeme_combat.h"
#include "../include/charger.h"
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



// CreatureMarine* creer_requin_tigre() {
//     CreatureMarine *requin = malloc(sizeof(CreatureMarine));
    
//     requin->id = 1;
//     strncpy(requin->nom, "Requin-Tigre", sizeof(requin->nom)-1);
//     requin->points_de_vie_max = 100;
//     requin->points_de_vie_actuels = 100;
//     requin->attaque_minimale = 10;
//     requin->attaque_maximale = 20;
//     requin->defense = 8;
//     requin->vitesse = 15;
//     strncpy(requin->effet_special, "saignement", sizeof(requin->effet_special)-1);
//     requin->est_vivant = 1;
    
//     return requin;
// }


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


void recommencer_jeu() {
    printf("🎮 Nouvelle partie!\n");
    
    
    Plongeur *nv_plongeur = nouveau_plongeur();
    Combat_plongeur *nv_plongeur_combat = nouveau_combat_plongeur(nv_plongeur);
    
    printf("✅ Plongeur créé avec les stats par défaut\n");
    
   
    afficher_plongeur(nv_plongeur);
    afficher_combat_plongeur(nv_plongeur_combat);
    
    // Ici tu peux appeler ta boucle principale de jeu
    // executer_jeu_principal(nv_plongeur_combat);
}

void game_over(Plongeur* plongeur, CreatureMarine* creatures, int nb_creatures) {
    printf(" Game Over! Sauvegarde de la partie...\n");
    
    // Sauvegarder l'état actuel (avec 0 PV)
    if (sauvegarder_jeu_complet("/mnt/c/Users/bcoqu/Documents/GROUPE-15/saves/game_over/savegame_gameover.json", plongeur, creatures, nb_creatures) == 0) {
        printf(" État de game over sauvegardé\n");
    }
    
    // Option: proposer de recommencer
    printf("Voulez-vous recommencer? (o/n): ");
    char choix = getchar();
    if (choix == 'o' || choix == 'O') {
        // Recréer une nouvelle partie
        recommencer_jeu();
    }
}

void quitter_jeu(Plongeur* plongeur, CreatureMarine* creatures, int nb_creatures) {
    printf("Sauvegarde en cours...\n");
    
    if (sauvegarder_jeu_complet("savegame.json", plongeur, creatures, nb_creatures) == 0) {
        printf("Partie sauvegardée avec succès!\n");
    } else {
        printf("Erreur lors de la sauvegarde\n");
    }
    
    // Libération mémoire
    free_plongeur(plongeur);
    // ... autres libérations
}

int main() {
    // Charger la sauvegarde directement dans tes structures
    SauvegardeJeu *sauvegarde = charger_sauvegarde_complete("./saves/oceandepths_save_v1.json");
    
    Plongeur *nv_plongeur;
    Combat_plongeur *nv_plongeur_combat;
    CreatureMarine creatures[MAX_CREATURES];
    int nb_creatures = 0;

    if (!sauvegarde) {
        printf("Création d'une nouvelle partie...\n");
        nv_plongeur = nouveau_plongeur();
        nv_plongeur_combat = nouveau_combat_plongeur(nv_plongeur);
        
        // Générer des créatures pour la nouvelle partie
        init_creatures_random(creatures, &nb_creatures, 150);
    } else {
        // Utiliser les structures de la sauvegarde
        nv_plongeur = sauvegarde->plongeur;
        nv_plongeur_combat = sauvegarde->combat_plongeur;
        // Note: Tu devras adapter pour utiliser sauvegarde->creatures et sauvegarde->nb_creatures
    }

    // Initialiser l'arène et l'interface
    Arene *jeu_arene = nouvelle_arene(1024 * 1024);
    GestionFenetre gf;
    nouvelle_gf(&gf, jeu_arene);
    
    int jeu_actif = 1;
    
    // Si sauvegarde chargée, combattre les créatures sauvegardées
    if (sauvegarde) {
        for (int i = 0; i < sauvegarde->nb_creatures && jeu_actif; i++) {
            if (sauvegarde->creatures[i].est_vivant) {
                printf("\n--- COMBAT contre %s ---\n", sauvegarde->creatures[i].nom);
                
                SystemeCombat *combat = creer_systeme_combat(&gf, sauvegarde->combat_plongeur, &sauvegarde->creatures[i]);
                executer_combat(combat);

                // Vérifier si le joueur est mort
                if (sauvegarde->plongeur->points_de_vie <= 0) {
                    game_over(sauvegarde->plongeur, sauvegarde->creatures, sauvegarde->nb_creatures);
                    jeu_actif = 0;
                }

                free(combat->interface);
                free(combat);
            }
        }
    } else {
        // Pour une nouvelle partie, tu devras implémenter ta propre logique de combat
        // ex: SystemeCombat *combat = creer_systeme_combat(&gf, nv_plongeur_combat, &creatures[0]);
        // executer_combat(combat);
        printf("Nouvelle partie - logique de combat à implémenter\n");
    }
    
    // Si le jeu est toujours actif, proposer de sauvegarder en quittant
    if (jeu_actif) {
        printf("\nVoulez-vous sauvegarder avant de quitter? (o/n): ");
        char choix = getchar();
        getchar(); // Consommer le \n
        
        if (choix == 'o' || choix == 'O') {
            if (sauvegarde) {
                quitter_jeu(sauvegarde->plongeur, sauvegarde->creatures, sauvegarde->nb_creatures);
            } else {
                quitter_jeu(nv_plongeur, creatures, nb_creatures);
            }
        }
    }
    
    // Nettoyage
    if (sauvegarde) {
        liberer_sauvegarde(sauvegarde);
    } else {
        // Libérer les structures de la nouvelle partie
        free_combat_plongeur(nv_plongeur_combat);
        free_plongeur(nv_plongeur);
    }
    
    arene_detruite(jeu_arene);
    return 0;
}