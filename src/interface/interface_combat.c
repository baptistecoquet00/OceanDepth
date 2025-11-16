#include "../../include/interface_combat.h"


// ========== INTERFACE ==========

InterfaceCombat* creer_interface_combat(GestionFenetre *gf) {
    InterfaceCombat *interface = malloc(sizeof(InterfaceCombat));
    interface->stats_win = gf_creer_fenetre(gf, 5, 3, 85, 6, "STATISTIQUES");
    interface->combat_win = gf_creer_fenetre(gf, 5, 10, 70, 12, "COMBAT SOUS-MARIN");
    interface->actions_win = gf_creer_fenetre(gf, 5, 24, 70, 6, "ACTIONS");
    
    
    // Bordures initiales
    fenetre_dessiner_bordure(interface->combat_win);
    fenetre_dessiner_bordure(interface->actions_win);
    fenetre_dessiner_bordure(interface->stats_win);
    
    return interface;
}

void mettre_a_jour_interface_combat(InterfaceCombat *interface, 
                                   Combat_plongeur *plongeur, 
                                   CreatureMarine *ennemi) {
    // Nettoyer et redessiner bordures
    fenetre_nettoyer(interface->combat_win);
    fenetre_nettoyer(interface->stats_win);
    fenetre_dessiner_bordure(interface->combat_win);
    fenetre_dessiner_bordure(interface->stats_win);
    
    // Contenu combat
    afficher_fenetref(interface->combat_win, 25, 3, "PLONGEUR VS %s", ennemi->nom);
    //afficher_fenetre(interface->combat_win, 28, 4, "♠        🦈");
    //afficher_fenetre(interface->combat_win, 25, 5, "---------> <---------");
    
    if (strcmp(ennemi->nom, "Requin") == 0) {
        afficher_fenetre(interface->combat_win, 28, 4, "♠        🦈");
        afficher_fenetre(interface->combat_win, 25, 5, "---------> <---------");
    }
    else if (strcmp(ennemi->nom, "Crabe Géant") == 0) {
        afficher_fenetre(interface->combat_win, 28, 4, "♠        🦀");
        afficher_fenetre(interface->combat_win, 25, 5, "---------> <---------");
    }
    else if (strcmp(ennemi->nom, "Méduse") == 0) {
        afficher_fenetre(interface->combat_win, 28, 4, "♠        🐙");
        afficher_fenetre(interface->combat_win, 25, 5, "---------> <---------");
    }
    else if (strcmp(ennemi->nom, "Kraken") == 0) {
        afficher_fenetre(interface->combat_win, 28, 4, "♠        🦑");
        afficher_fenetre(interface->combat_win, 25, 5, "---------> <---------");
    }
    else if (strcmp(ennemi->nom, "Poisson-Épée") == 0){
        // Valeur par défaut si le monstre n'est pas reconnu
        afficher_fenetre(interface->combat_win, 28, 4, "♠        🐟");
        afficher_fenetre(interface->combat_win, 25, 5, "---------> <---------");
    }
    else {
        // Valeur par défaut si le monstre n'est pas reconnu
        afficher_fenetre(interface->combat_win, 28, 4, "♠        🐟");
        afficher_fenetre(interface->combat_win, 25, 5, "---------> <---------");
    }

    // Stats dynamiques combat
    // afficher_fenetref(interface->combat_win, 2, 7, "PV Joueur: %d/%d", 
    //                  plongeur->gestion_fatigue_vie->points_de_vie,
    //                  plongeur->gestion_fatigue_vie->points_de_vie_max);
    afficher_fenetref(interface->combat_win, 2, 7, "PV %s: %d/%d", 
                     ennemi->nom, ennemi->points_de_vie_actuels, ennemi->points_de_vie_max);
    afficher_fenetref(interface->combat_win, 2, 8, "Effet: %s", ennemi->effet_special);
    
    // Stats joueur détaillées
    mettre_a_jour_stats_joueur(interface->stats_win, plongeur->gestion_fatigue_vie);
}

void afficher_actions_disponibles(InterfaceCombat *interface) {
    fenetre_nettoyer(interface->actions_win);
    fenetre_dessiner_bordure(interface->actions_win);
    
    afficher_fenetre(interface->actions_win, 2, 1, "CHOISISSEZ UNE ACTION:");
    afficher_fenetre(interface->actions_win, 2, 2, "1. Attaque    2. Competence    3. Defendre");
    afficher_fenetre(interface->actions_win, 2, 3, "4. Objet      5. Fuir          0. Terminer tour");
}

void afficher_message_combat(InterfaceCombat *interface, const char *format, ...) {
    if (!interface || !interface->actions_win) return;
    
    fenetre_nettoyer(interface->actions_win);
    fenetre_dessiner_bordure(interface->actions_win);
    
    // Buffer pour formater le message
    char buffer[256];
    va_list args;
    
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    // Afficher le message formaté
    afficher_fenetref(interface->actions_win, 2, 1, "%s", buffer);
    
    // Garder les actions disponibles en dessous
    afficher_fenetre(interface->actions_win, 2, 2, "1. Attaque    2. Competence    3. Defendre");
    afficher_fenetre(interface->actions_win, 2, 3, "4. Objet      5. Fuir          0. Terminer tour");
}

void detruire_interface_combat(InterfaceCombat *interface) {
    // Les fenêtres sont gérées par l'arène, pas besoin de free
    free(interface);
}