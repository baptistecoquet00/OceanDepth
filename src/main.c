#include "../include/joueur.h"
#include "../include/utilitaire.h"
#include "../include/interface_combat.h"
#include "../include/systeme_combat.h"
#include "../include/charger.h"
#include "../include/sauvegarde.h"
#include "../include/carte.h"
#include "../include/interface_carte.h"

// main.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <locale.h>
#include <termios.h>

#define ARENE_IMPLEMENTATION
#include "../include/gm.h"
#include "../include/systeme_fenetre.h"
#include "../include/interface_stats_joueur.h"
#include "../include/combat.h"

// Fonction pour configurer le terminal en mode non-canonique
void configurer_terminal() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Fonction pour restaurer le terminal
void restaurer_terminal() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Fonction pour lire un caractère sans bloquer
int lire_caractere() {
    struct termios oldt, newt;
    int ch;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void afficher_menu_principal() {
    printf("\033[2J\033[1;1H"); // Clear screen
    printf("=== OCEAN DEPTHS ===\n");
    printf("1. Nouvelle partie\n");
    printf("2. Charger partie\n");
    printf("3. Quitter\n");
    printf("Choix: ");
    fflush(stdout);
}

void game_over(Plongeur* plongeur, CreatureMarine* creatures, int nb_creatures) {
    printf("Game Over! Sauvegarde de la partie...\n");
    
    if (sauvegarder_jeu_complet("./saves/game_over/savegame_gameover.json", plongeur, creatures, nb_creatures) == 0) {
        printf("État de game over sauvegardé\n");
    }
    
    printf("Voulez-vous recommencer? (o/n): ");
    char choix = getchar();
    if (choix == 'o' || choix == 'O') {
        printf("Redémarrage...\n");
    } else {
        printf("Retour au menu principal...\n");
    }
}





void quitter_jeu(Plongeur* plongeur, CreatureMarine* creatures, int nb_creatures) {
    printf("Sauvegarde en cours...\n");
    
    if (sauvegarder_jeu_complet("./saves/savegame.json", plongeur, creatures, nb_creatures) == 0) {
        printf("✅ Partie sauvegardée avec succès!\n");
    } else {
        printf("❌ Erreur lors de la sauvegarde\n");
    }
}

void executer_nouvelle_partie(GestionFenetre *gf, Arene *arene) {
    printf("🎮 Démarrage d'une nouvelle partie...\n");
    
    // Créer le joueur
    Plongeur *nv_plongeur = nouveau_plongeur();
    Combat_plongeur *nv_plongeur_combat = nouveau_combat_plongeur(nv_plongeur);
    
    // Créer la carte
    CarteOcean *carte = creer_carte(arene);
    InterfaceCarte *interface_carte = creer_interface_carte(gf);
    
    printf("✅ Monde initialisé - Prêt à explorer!\n");
    printf("Appuyez sur une touche pour continuer...\n");
    getchar();
    
    // Boucle principale de navigation
    int navigation_actif = 1;
    while (navigation_actif) {
        // Utiliser le système de fenêtres
        mettre_a_jour_interface_carte(interface_carte, carte);
        gf_rendu(gf);
        
        printf("Commande (ZQSD/E/R): ");
        fflush(stdout);
        
        // Lire l'entrée
        char choix = lire_caractere();
        printf("\n");
        
        switch(choix) {
            case 'z': 
            case 'Z':
                if (deplacement_possible(carte, 0, -1)) {
                    deplacer_joueur(carte, 0, -1);
                    printf("Déplacement vers le nord\n");
                } else {
                    printf("Déplacement impossible!\n");
                }
                break;
            case 's': 
            case 'S':
                if (deplacement_possible(carte, 0, 1)) {
                    deplacer_joueur(carte, 0, 1);
                    printf("Déplacement vers le sud\n");
                } else {
                    printf("Déplacement impossible!\n");
                }
                break;
            case 'q': 
            case 'Q':
                if (deplacement_possible(carte, -1, 0)) {
                    deplacer_joueur(carte, -1, 0);
                    printf("Déplacement vers l'ouest\n");
                } else {
                    printf("Déplacement impossible!\n");
                }
                break;
            case 'd': 
            case 'D':
                if (deplacement_possible(carte, 1, 0)) {
                    deplacer_joueur(carte, 1, 0);
                    printf("Déplacement vers l'est\n");
                } else {
                    printf("Déplacement impossible!\n");
                }
                break;
            case 'e':
            case 'E':
                {
                    static int combat_count = 0; // Compteur de combats
                    
                   
                    printf("=== COMBAT #%d ===\n", combat_count);

                    ZoneCarte *zone = get_zone_actuelle(carte);
                    printf("Interaction avec: %s\n", zone->nom);
                    
                    if (zone->nb_ennemis > 0) {
                        combat_count++;
                        printf("⚔️  Combat déclenché contre %s!\n", zone->nom);
                        
                        // Générer des créatures pour cette zone
                        CreatureMarine creatures[MAX_CREATURES];
                        int nb_creatures;
                        init_creatures_random(creatures, &nb_creatures, zone->profondeur);
                        
                        if (nb_creatures > 0 && creatures[0].nom[0] != '\0') {
                            printf("Vous combattez un %s!\n", creatures[0].nom);
                            
                            //  CRÉER UN GESTIONNAIRE DE FENÊTRES SÉPARÉ POUR LE COMBAT 
                             GestionFenetre *gf_combat = malloc(sizeof(GestionFenetre));
                            nouvelle_gf(gf_combat, arene);
                            
                            // Clear screen complet
                            printf("\033[2J\033[1;1H");
                            printf("=== DÉBUT DU COMBAT ===\n");
                            sleep(2);
                            
                            // Utiliser le NOUVEAU gestionnaire pour le combat
                            SystemeCombat *combat = creer_systeme_combat(gf_combat, nv_plongeur_combat, &creatures[0]);
                            executer_combat(combat);
                            
                            bool joueur_est_mort = false;

                            // Vérifier si le joueur est mort
                            if (nv_plongeur->points_de_vie <= 0) {
                                printf("💀 Game Over! Vous avez été vaincu...\n");
                                
                                game_over(nv_plongeur, creatures, nb_creatures);
                                
                                navigation_actif = 0;
                                
                                //free(gf_combat);
                                joueur_est_mort = true;

                                if (interface_carte) detruire_interface_carte(interface_carte);
                                free_combat_plongeur(nv_plongeur_combat);
                                free_plongeur(nv_plongeur);
                                return;
                            } else if (!creatures[0].est_vivant) {
                                // Victoire - récompenses
                                printf("✅ Vous avez vaincu le %s!\n", creatures[0].nom);
                                nv_plongeur->perles += 20;
                                printf("+20 perles! Total: %d perles\n", nv_plongeur->perles);
                                
                                // Marquer la zone comme pacifiée
                                zone->nb_ennemis = 0;
                            }
                            
                            // Nettoyer le système de combat
                            detruire_systeme_combat(combat);
                            
                            //  DÉTRUIRE le gestionnaire de combat
                            free(gf_combat); 
                            // (Les fenêtres sont dans l'arène, pas besoin de les détruire manuellement)
                            
                            if (joueur_est_mort) {
                                if (interface_carte) detruire_interface_carte(interface_carte);
                                free_combat_plongeur(nv_plongeur_combat);
                                free_plongeur(nv_plongeur);
                                return;
                            }

                            // Clear screen complet avant de retourner à la carte
                            printf("\033[2J\033[1;1H");
                            printf("Retour à la carte...\n");
                            sleep(1);
                            
                            //  RECRÉER COMPLÈTEMENT l'interface carte
                            // detruire_interface_carte(interface_carte);
                            // interface_carte = creer_interface_carte(gf);
                            
                            printf("\033[2J\033[1;1H");
                            printf("Retour à la carte...\n");
                            sleep(1);

                            // Juste mettre à jour l'affichage, pas recréer
                            mettre_a_jour_interface_carte(interface_carte, carte);
                            gf_rendu(gf);

                            // Rafraîchir tout
                            mettre_a_jour_interface_carte(interface_carte, carte);
                            gf_rendu(gf);
                        }
                    }else if (zone->type == TYPE_BASE) {
                        printf("🏠 Retour à la base - Sauvegarde automatique\n");
                        // Restaurer PV et oxygène
                        nv_plongeur->points_de_vie = nv_plongeur->points_de_vie_max;
                        nv_plongeur->niveau_oxygene = nv_plongeur->niveau_oxygene_max;
                        nv_plongeur->niveau_fatigue = 0;
                        printf("PV et oxygène restaurés!\n");
                        
                        // Sauvegarder la partie
                        if (sauvegarder_jeu_complet("./saves/savegame_auto.json", nv_plongeur, NULL, 0) == 0) {
                            printf("💾 Partie sauvegardée!\n");
                        }
                    } else if (zone->type == TYPE_BATEAU) {
                        printf("🛍️  Bienvenue au magasin!\n");
                        printf("Vous avez %d perles\n", nv_plongeur->perles);
                        printf("(Fonctionnalité à implémenter)\n");
                    } else if (zone->type == TYPE_EPAVE) {
                        printf("💰 Vous trouvez un trésor dans l'épave!\n");
                        int perles_trouvees = 10 + rand() % 40;
                        nv_plongeur->perles += perles_trouvees;
                        printf("+%d perles! Total: %d perles\n", perles_trouvees, nv_plongeur->perles);
                    } else if (zone->type == TYPE_GROTTE) {
                        printf("🕳️  Vous explorez la grotte...\n");
                        printf("Vous trouvez un équipement spécial!\n");
                        // Amélioration des stats
                        nv_plongeur->points_de_vie_max += 10;
                        nv_plongeur->points_de_vie = nv_plongeur->points_de_vie_max;
                        printf("+10 PV maximum! PV: %d/%d\n", 
                               nv_plongeur->points_de_vie, nv_plongeur->points_de_vie_max);
                        if (sauvegarder_jeu_complet("./saves/savegame_auto.json", nv_plongeur, NULL, 0) == 0) {
                            printf("💾 Progression sauvegardée automatiquement!\n");
                        } else {
                            printf("❌ Erreur lors de la sauvegarde automatique\n");
                        }
                    } else {
                        printf("Rien d'intéressant ici...\n");
                    }
                    printf("Appuyez sur une touche pour continuer...");
                    getchar();
                }
                break;
            case 'r':
            case 'R':
                navigation_actif = 0;
                printf("Retour au menu principal...\n");
                break;
            default:
                printf("Commande invalide. Utilisez ZQSD pour vous déplacer, E pour interagir, R pour quitter.\n");
                printf("Appuyez sur une touche pour continuer...");
                getchar();
                break;
        }
        
        // Petite pause
        sleep(1);
    }
    
    // Nettoyage
    if (interface_carte) detruire_interface_carte(interface_carte);
    free_combat_plongeur(nv_plongeur_combat);
    free_plongeur(nv_plongeur);
}


void executer_partie_chargee(GestionFenetre *gf, Arene *arene) {
    printf("Chargement de la sauvegarde...\n");
    
    SauvegardeJeu *sauvegarde = charger_sauvegarde_complete("./saves/oceandepths_save_v1.json");
    
    if (!sauvegarde) {
        printf("❌ Erreur: Impossible de charger la sauvegarde\n");
        printf("Création d'une nouvelle partie à la place...\n");
        sleep(2);
        executer_nouvelle_partie(gf, arene);
        return;
    }
    
    printf("✅ Sauvegarde chargée - Reprise de la partie\n");
    printf("Joueur: PV: %d/%d, Oxygène: %d/%d, Perles: %d\n", 
           sauvegarde->plongeur->points_de_vie,
           sauvegarde->plongeur->points_de_vie_max,
           sauvegarde->plongeur->niveau_oxygene,
           sauvegarde->plongeur->niveau_oxygene_max,
           sauvegarde->plongeur->perles);
    printf("Appuyez sur une touche pour continuer...\n");
    getchar();
    
    // ⭐⭐ UTILISER RÉELLEMENT LA SAUVEGARDE ⭐⭐
    printf("🎮 Démarrage de la partie chargée...\n");
    
    // Créer la carte
    CarteOcean *carte = creer_carte(arene);
    InterfaceCarte *interface_carte = creer_interface_carte(gf);
    
    printf("✅ Monde chargé - Prêt à explorer!\n");
    printf("Appuyez sur une touche pour continuer...\n");
    getchar();
    
    // Boucle principale de navigation AVEC LES DONNÉES CHARGÉES
    int navigation_actif = 1;
    while (navigation_actif) {
        mettre_a_jour_interface_carte(interface_carte, carte);
        gf_rendu(gf);
        
        printf("Commande (ZQSD/E/R): ");
        fflush(stdout);
        
        char choix = lire_caractere();
        printf("\n");
        
        switch(choix) {
            case 'z': 
            case 'Z':
                if (deplacement_possible(carte, 0, -1)) {
                    deplacer_joueur(carte, 0, -1);
                    printf("Déplacement vers le nord\n");
                } else {
                    printf("Déplacement impossible!\n");
                }
                break;
            case 's': 
            case 'S':
                if (deplacement_possible(carte, 0, 1)) {
                    deplacer_joueur(carte, 0, 1);
                    printf("Déplacement vers le sud\n");
                } else {
                    printf("Déplacement impossible!\n");
                }
                break;
            case 'q': 
            case 'Q':
                if (deplacement_possible(carte, -1, 0)) {
                    deplacer_joueur(carte, -1, 0);
                    printf("Déplacement vers l'ouest\n");
                } else {
                    printf("Déplacement impossible!\n");
                }
                break;
            case 'd': 
            case 'D':
                if (deplacement_possible(carte, 1, 0)) {
                    deplacer_joueur(carte, 1, 0);
                    printf("Déplacement vers l'est\n");
                } else {
                    printf("Déplacement impossible!\n");
                }
                break;
            case 'e':
            case 'E':
                {
                    ZoneCarte *zone = get_zone_actuelle(carte);
                    printf("Interaction avec: %s\n", zone->nom);
                    
                    if (zone->nb_ennemis > 0) {
                        printf("⚔️  Combat déclenché contre %s!\n", zone->nom);
                        
                        CreatureMarine creatures[MAX_CREATURES];
                        int nb_creatures;
                        init_creatures_random(creatures, &nb_creatures, zone->profondeur);
                        
                        if (nb_creatures > 0 && creatures[0].nom[0] != '\0') {
                            printf("Vous combattez un %s!\n", creatures[0].nom);
                            
                            GestionFenetre *gf_combat = malloc(sizeof(GestionFenetre));
                            nouvelle_gf(gf_combat, arene);
                            
                            printf("\033[2J\033[1;1H");
                            printf("=== DÉBUT DU COMBAT ===\n");
                            sleep(2);
                            
                            SystemeCombat *combat = creer_systeme_combat(gf_combat, sauvegarde->combat_plongeur, &creatures[0]);
                            executer_combat(combat);
                            
                            bool joueur_est_mort = (sauvegarde->plongeur->points_de_vie <= 0);
                            bool ennemi_est_mort = !creatures[0].est_vivant;
                            
                            if (joueur_est_mort) {
                                printf("💀 Game Over! Vous avez été vaincu...\n");
                                game_over(sauvegarde->plongeur, creatures, nb_creatures);
                                navigation_actif = 0;
                            } else if (ennemi_est_mort) {
                                printf("✅ Vous avez vaincu le %s!\n", creatures[0].nom);
                                sauvegarde->plongeur->perles += 20;
                                printf("+20 perles! Total: %d perles\n", sauvegarde->plongeur->perles);
                                zone->nb_ennemis = 0;
                            }
                            
                            detruire_systeme_combat(combat);
                            free(gf_combat);
                            
                            if (joueur_est_mort) {
                                if (interface_carte) detruire_interface_carte(interface_carte);
                                liberer_sauvegarde(sauvegarde);
                                return;
                            }
                            
                            printf("\033[2J\033[1;1H");
                            printf("Retour à la carte...\n");
                            sleep(1);
                            
                            mettre_a_jour_interface_carte(interface_carte, carte);
                            gf_rendu(gf);
                        }
                    } else {
                        // Interactions avec les autres zones
                        if (zone->type == TYPE_BASE) {
                            printf("🏠 Retour à la base\n");
                            sauvegarde->plongeur->points_de_vie = sauvegarde->plongeur->points_de_vie_max;
                            sauvegarde->plongeur->niveau_oxygene = sauvegarde->plongeur->niveau_oxygene_max;
                            sauvegarde->plongeur->niveau_fatigue = 0;
                            printf("PV et oxygène restaurés!\n");
                            
                            if (sauvegarder_jeu_complet("./saves/savegame_auto.json", sauvegarde->plongeur, NULL, 0) == 0) {
                                printf("💾 Partie sauvegardée!\n");
                            }
                        }
                        // ... autres interactions
                    }
                    printf("Appuyez sur une touche pour continuer...");
                    getchar();
                }
                break;
            case 'r':
            case 'R':
                navigation_actif = 0;
                printf("Retour au menu principal...\n");
                break;
            default:
                printf("Commande invalide\n");
                printf("Appuyez sur une touche pour continuer...");
                getchar();
                break;
        }
        
        sleep(1);
    }
    
    // Nettoyage
    if (interface_carte) detruire_interface_carte(interface_carte);
    liberer_sauvegarde(sauvegarde);
}

int main() {
    // Au début de main()
    printf("Début du jeu - Mémoire allouée: %u GB\n", (1024 * 1024 * 1024) / (1024 * 1024 * 1024));
    // Initialisation pour supporter les émojis
    setlocale(LC_ALL, "en_US.UTF-8");
    
    // Initialisation aléatoire
    srand(time(NULL));
    
    // Configurer le terminal
    configurer_terminal();
    
    // Initialisation de l'arène et du système de fenêtres
    Arene *jeu_arene = nouvelle_arene(1024 * 1024 * 1024); // 1GB - NO LIMITS! 
    GestionFenetre gf;
    nouvelle_gf(&gf, jeu_arene);
    
    printf("=== OCEAN DEPTHS - Jeu d'aventure sous-marine ===\n");
    printf("Initialisation terminée. Appuyez sur une touche pour continuer...\n");
    getchar();
    
    // Menu principal
    int jeu_actif = 1;
    while (jeu_actif) {
        afficher_menu_principal();
        
        char choix = lire_caractere();
        printf("\n");
        
        switch(choix) {
            case '1':
                printf("Lancement nouvelle partie...\n");
                sleep(1);
                executer_nouvelle_partie(&gf, jeu_arene);
                break;
            case '2':
                printf("Chargement partie...\n");
                sleep(1);
                executer_partie_chargee(&gf, jeu_arene);
                break;
            case '3':
                jeu_actif = 0;
                printf("Au revoir!\n");
                break;
            default:
                printf("❌ Choix invalide. Appuyez sur une touche...\n");
                getchar();
                break;
        }
    }
    
    // Nettoyage final
    restaurer_terminal();
    arene_detruite(jeu_arene);
    printf("Jeu terminé. Merci d'avoir joué !\n");
    return 0;
}