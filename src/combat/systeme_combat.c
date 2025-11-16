#include "../../include/systeme_combat.h"

#include "../../include/interface_stats_joueur.h"



// ========== SYSTÈME ==========

SystemeCombat* creer_systeme_combat(GestionFenetre *gf, Combat_plongeur *plongeur, CreatureMarine *ennemi) {
    SystemeCombat *systeme = malloc(sizeof(SystemeCombat));
    
    systeme->gf = gf;
    systeme->plongeur = plongeur;
    systeme->ennemi = ennemi;
    systeme->interface = creer_interface_combat(gf);
    systeme->combat_actif = true;
    
    // Initialisation du tour avec TON header
    nouveau_tour(&systeme->tour);
    
    return systeme;
}

void executer_tour_joueur(SystemeCombat *systeme) {
    int nombre_attaques_ce_tour = 0;
    int attaques_effectuees = 0;
    int attaques_max = systeme->plongeur->nb_attaque_par_tour;
    
    // Boucle pour les choix multiples dans le même tour
    while (attaques_effectuees < attaques_max && systeme->combat_actif) {
        // Mise à jour de l'interface pour montrer les attaques restantes
        mettre_a_jour_interface_combat(systeme->interface, systeme->plongeur, systeme->ennemi);
        
        afficher_fenetref(systeme->interface->actions_win, 2, 1, 
                        "Action %d/%d - Choix:", 
                        attaques_effectuees + 1, attaques_max);
        afficher_actions_disponibles(systeme->interface);
        
        gf_rendu(systeme->gf);
        
        printf("Action %d/%d: ", attaques_effectuees + 1, attaques_max);
        int action = getchar() - '0';
        getchar();
        
        if (action == 0) {
            // Option pour terminer le tour plus tôt
            break;
        }
        
        bool est_attaque = false;
        
        switch(action) {
            case 1: // Attaque normale
                systeme->plongeur->attaque_normale = true;
                int degats = 8 + rand() % 5;
                systeme->ennemi->points_de_vie_actuels -= degats;
                afficher_message_combat(systeme->interface, "Attaque %d: -%d PV", 
                                       attaques_effectuees + 1, degats);
                est_attaque = true;
                break;
                
            case 2: // Compétence
                systeme->plongeur->competence_special = true;
                int degats_speciaux = 15 + rand() % 10;
                systeme->ennemi->points_de_vie_actuels -= degats_speciaux;
                afficher_message_combat(systeme->interface, "Competence %d: -%d PV", 
                                       attaques_effectuees + 1, degats_speciaux);
                est_attaque = true;
                break;
                
            case 3: // Défendre (une seule fois par tour)
                if (attaques_effectuees == 0) {
                    afficher_message_combat(systeme->interface, "Vous vous defendez!");
                    // Défense active pour tout le tour
                }
                break;
                
            case 4: // Objet (une seule fois par tour)
                if (attaques_effectuees == 0) {
                    afficher_message_combat(systeme->interface, "Vous utilisez un objet!");
                }
                break;
                
            case 5: // Fuir (termine le tour)
                afficher_message_combat(systeme->interface, "Vous tentez de fuir...");
                systeme->combat_actif = false;
                systeme->plongeur->a_fui = true;
                attaques_effectuees = attaques_max; // Force la fin du tour
                break;
        }
        
        if (est_attaque) {
            nombre_attaques_ce_tour++;
            attaques_effectuees++;
        } else {
            // Pour défense/objet, on compte comme une action mais pas une attaque
            attaques_effectuees++;
        }
        
        // Rendu après chaque action pour voir l'effet
        gf_rendu(systeme->gf);
        sleep(1);
    }
    
    //  MISE À JOUR FATIGUE APRÈS TOUTES LES ACTIONS DU TOUR 
    combat_plongeur_calcul_fatigue(systeme->plongeur, nombre_attaques_ce_tour);
    
    // Gestion oxygène
    combat_plongeur_gestion_oxygene(systeme->plongeur);
    
    // Réinitialiser flags
    systeme->plongeur->attaque_normale = false;
    systeme->plongeur->competence_special = false;
}

void executer_tour_ennemi(SystemeCombat *systeme) {
    // Attaque de l'ennemi
    int degats_ennemi = systeme->ennemi->attaque_minimale + 
                       rand() % (systeme->ennemi->attaque_maximale - systeme->ennemi->attaque_minimale + 1);
    
    systeme->plongeur->gestion_fatigue_vie->points_de_vie -= degats_ennemi;
    
    afficher_message_combat(systeme->interface, "%s attaque! -%d PV", 
                           systeme->ennemi->nom, degats_ennemi);
    
    // Gestion oxygène
    //combat_plongeur_gestion_oxygene(systeme->plongeur);
}

bool verifier_fin_combat(SystemeCombat *systeme) {
    if (systeme->plongeur->gestion_fatigue_vie->points_de_vie <= 0) {
        afficher_message_combat(systeme->interface, "\e[1;31mVOUS ETES MORT!\e[0m");
        return true;
    }
    if (systeme->ennemi->points_de_vie_actuels <= 0) {
        afficher_message_combat(systeme->interface, "\e[1;32mVictoire! %s vaincu\e[0m", systeme->ennemi->nom);
        return true;
    }
    return false;
}

void executer_combat(SystemeCombat *systeme) {
    while (systeme->combat_actif) {
        // Mise à jour interface
        mettre_a_jour_interface_combat(systeme->interface, systeme->plongeur, systeme->ennemi);
        
        if (tour_est_au_joueur(&systeme->tour)) {
            if (tour_attente_action(&systeme->tour)) {
                executer_tour_joueur(systeme);
                tour_action_effectuee(&systeme->tour);
            } else {
                tour_passer_creature(&systeme->tour);
            }
        } else {
            executer_tour_ennemi(systeme);
            tour_suivant(&systeme->tour);
        }
        
        // Vérifier fin
        if (verifier_fin_combat(systeme)) {
            systeme->combat_actif = false;
        }
        
        // Rendu
        gf_rendu(systeme->gf);
        sleep(2);
    }
    
    
    printf("\033[2J\033[1;1H"); // Clear screen
    printf("=== COMBAT TERMINÉ ===\n");
    
    // Afficher le résultat final
    if (systeme->plongeur->gestion_fatigue_vie->points_de_vie <= 0) {
        printf("💀 Vous avez été vaincu!\n");
    } else if (systeme->ennemi->points_de_vie_actuels <= 0) {
        printf("✅ Victoire! %s vaincu\n", systeme->ennemi->nom);
        printf("+20 perles!\n");
    }
    
    printf("Appuyez sur une touche pour continuer...");
    getchar(); // Attendre une touche
    
    
    fenetre_nettoyer(systeme->interface->combat_win);
    fenetre_nettoyer(systeme->interface->actions_win);
    fenetre_nettoyer(systeme->interface->stats_win);
}

// Variables pour gérer la position courante dans la fenêtre
static int curseur_x = 0;
static int curseur_y = 0;

void deplacer_curseur_fenetre(Fenetre *fenetre, int y, int x) {
    if (!fenetre) return;
    
    // S'assurer que les coordonnées sont dans les limites
    curseur_x = (x < 0) ? 0 : (x >= fenetre->largeur - 2) ? fenetre->largeur - 2 : x;
    curseur_y = (y < 0) ? 0 : (y >= fenetre->hauteur - 2) ? fenetre->hauteur - 2 : y;
    
    // Ajuster pour la bordure (zone interne commence à 1,1)
    curseur_x += 1;
    curseur_y += 1;
}

void afficher_texte_fenetre(Fenetre *fenetre, const char *texte) {
    if (!fenetre || !fenetre->tampon || !texte) return;
    
    int pos = curseur_y * fenetre->largeur + curseur_x;
    int max_chars = fenetre->largeur - curseur_x;
    
    int chars_a_copier = strlen(texte);
    if (chars_a_copier > max_chars) {
        chars_a_copier = max_chars;
    }
    
    // Copier le texte à la position courante
    memcpy(&fenetre->tampon[pos], texte, chars_a_copier);
    
    // Avancer le curseur
    curseur_x += chars_a_copier;
    
    // Si on dépasse la largeur, passer à la ligne suivante
    if (curseur_x >= fenetre->largeur - 1) {
        curseur_x = 1; // Retour au début (après bordure gauche)
        curseur_y++;
        
        // Si on dépasse la hauteur, rester sur la dernière ligne
        if (curseur_y >= fenetre->hauteur - 1) {
            curseur_y = fenetre->hauteur - 2;
        }
    }
}

void debut_attribut(Fenetre *fenetre, int attribut) {
    // Dans un système simple sans vrai support couleur, on ignore les attributs
    // Mais on garde la fonction pour la compatibilité
    (void)fenetre;
    (void)attribut;
}

void fin_attribut(Fenetre *fenetre, int attribut) {
    // Même chose - ignoré dans cette implémentation simple
    (void)fenetre;
    (void)attribut;
}

void detruire_systeme_combat(SystemeCombat *systeme) {
    detruire_interface_combat(systeme->interface);
    free(systeme);
}