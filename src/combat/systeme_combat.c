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
                // ⭐⭐ SUPPRIMER l'appel à afficher_actions_disponibles() ici
                // ⭐⭐ SUPPRIMER le getchar() ici
                
                // ⭐⭐ APPEL DIRECT SANS PARAMÈTRE ⭐⭐
                executer_tour_joueur(systeme);  // ⬅️ Plus de paramètre 'action'
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
}

void detruire_systeme_combat(SystemeCombat *systeme) {
    detruire_interface_combat(systeme->interface);
    free(systeme);
}