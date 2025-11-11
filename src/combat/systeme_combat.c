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

void executer_tour_joueur(SystemeCombat *systeme, int action) {
    switch(action) {
        case 1: // Attaque normale
            systeme->plongeur->attaque_normale = true;
            int degats = 8 + rand() % 5;
            systeme->ennemi->points_de_vie_actuels -= degats;
            afficher_message_combat(systeme->interface, "Attaque normale! -%d PV", degats);
            break;
            
        case 2: // Compétence
            systeme->plongeur->competence_special = true;
            int degats_speciaux = 15 + rand() % 10;
            systeme->ennemi->points_de_vie_actuels -= degats_speciaux;
            afficher_message_combat(systeme->interface, "Competence speciale! -%d PV", degats_speciaux);
            break;
            
        case 3: // Défendre
            afficher_message_combat(systeme->interface, "Vous vous defendez!");
            break;
            
        case 4: // Objet
            afficher_message_combat(systeme->interface, "Vous utilisez un objet!");
            break;
            
        case 5: // Fuir
            afficher_message_combat(systeme->interface, "Vous tentez de fuir...");
            systeme->combat_actif = false;
            break;
    }
    
    // Gestion oxygène après action
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
                afficher_actions_disponibles(systeme->interface);
                printf("Tour %d - Joueur: ", tour_numero(&systeme->tour));
                int action = getchar() - '0';
                getchar();
                
                if (action >= 1 && action <= 5) {
                    executer_tour_joueur(systeme, action);
                    tour_action_effectuee(&systeme->tour);
                }
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