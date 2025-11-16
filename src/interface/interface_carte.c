#include "../../include/interface_carte.h"
#include <string.h>
#include <locale.h>
#include <stdarg.h>

// Fonction utilitaire pour centrer le texte
static void centrer_texte(Fenetre *win, int y, const char *texte) {
    int largeur = win->largeur;
    int debut = (largeur - strlen(texte)) / 2;
    // Note: Vous devrez adapter selon votre système d'attributs
    deplacer_curseur_fenetre(win, y, debut);
    afficher_texte_fenetre(win, texte);
}

// Fonction pour obtenir la couleur d'une zone 
static int get_couleur_zone(const ZoneCarte *zone) {
    if (!zone->decouverte) return COULEUR_MAGENTA;
    
    switch(zone->type) {
        case TYPE_BASE: return COULEUR_VERT;
        case TYPE_BATEAU: return COULEUR_CYAN;
        case TYPE_EPAVE: return COULEUR_JAUNE;
        case TYPE_RECIF: return COULEUR_ROUGE;
        case TYPE_ALGUES: return COULEUR_VERT;
        case TYPE_GROTTE: return COULEUR_BLANC;
        case TYPE_REQUIN: return COULEUR_ROUGE;
        case TYPE_KRAKEN: return COULEUR_ROUGE;
        case TYPE_OCEAN: return COULEUR_BLEU;
        case TYPE_VIDE: return COULEUR_BLANC;
        case TYPE_INCONNU: return COULEUR_MAGENTA;
        default: return COULEUR_BLANC;
    }
}

InterfaceCarte* creer_interface_carte(GestionFenetre *gf) {
    InterfaceCarte *interface = malloc(sizeof(InterfaceCarte));
    if (!interface) return NULL;
    
    // Fenêtre principale pour la carte
    interface->carte_win = gf_creer_fenetre(gf, 0, 0, 80, 25, "Carte Ocean");
    
    // Fenêtre pour les informations de zone
    interface->info_win = gf_creer_fenetre(gf, 0, 25, 80, 10, "Informations Zone");
    
    // Fenêtre pour les commandes
    interface->commande_win = gf_creer_fenetre(gf, 0, 35, 80, 5, "Commandes");
    
    interface->visible = true;  // ⭐⭐ Visible par défaut ⭐⭐
    
    return interface;
}

void afficher_carte(InterfaceCarte *interface, const CarteOcean *carte) {
    Fenetre *win = interface->carte_win;
    fenetre_nettoyer(win);
    
    // Titre
    centrer_texte(win, 0, "CARTOGRAPHIE OCÉANIQUE  SECTEUR PACIFIQUE");
    
    // Ligne supérieure du tableau
    int y = 2;
    deplacer_curseur_fenetre(win, y, 0);
    afficher_texte_fenetre(win, "┌─────────┬─────────┬─────────┬─────────┬────────────┐");
    
    // En-têtes des colonnes
    y++;
    deplacer_curseur_fenetre(win, y, 0);
    afficher_texte_fenetre(win, "│         │         │         │         │            │");
    
    // Affichage des zones
    for (int ligne = 0; ligne < CARTE_HAUTEUR; ligne++) {
        y++;
        
        // Ligne d'emoji
        deplacer_curseur_fenetre(win, y, 0);
        afficher_texte_fenetre(win, "│");
        
        for (int colonne = 0; colonne < CARTE_LARGEUR; colonne++) {
            const ZoneCarte *zone = &carte->zones[ligne][colonne];
            int couleur = get_couleur_zone(zone);
            
            // Afficher l'emoji (ou "?" si non découvert)
            if (zone->decouverte || ligne == 0) { // La surface est toujours visible
                afficher_texte_fenetre(win, " ");
                afficher_texte_fenetre(win, zone->emoji);
                afficher_texte_fenetre(win, " ");
            } else {
                afficher_texte_fenetre(win, " ❓ ");
            }
            
            afficher_texte_fenetre(win, "│");
        }
        
        // Affichage de la profondeur
        char profondeur[16];
        switch(ligne) {
            case 0: strcpy(profondeur, "SURFACE │ 0m"); break;
            case 1: strcpy(profondeur, "ZONE 1  │ 50m"); break;
            case 2: strcpy(profondeur, "ZONE 2  │ 150m"); break;
            case 3: strcpy(profondeur, "ZONE 3  │ 300m"); break;
        }
        afficher_texte_fenetre(win, profondeur);
        
        y++;
        
        // Ligne de description
        deplacer_curseur_fenetre(win, y, 0);
        afficher_texte_fenetre(win, "│");
        
        for (int colonne = 0; colonne < CARTE_LARGEUR; colonne++) {
            const ZoneCarte *zone = &carte->zones[ligne][colonne];
            
            // Afficher le nom (tronqué si nécessaire)
            char nom_affiche[10];
            if (zone->decouverte || ligne == 0) {
                strncpy(nom_affiche, zone->nom, 7);
                nom_affiche[7] = '\0';
            } else {
                strcpy(nom_affiche, "???");
            }
            
            afficher_texte_fenetre(win, " ");
            afficher_texte_fenetre(win, nom_affiche);
            afficher_texte_fenetre(win, " ");
            
            afficher_texte_fenetre(win, "│");
        }
        
        // Espace vide pour l'alignement
        afficher_texte_fenetre(win, "            │");
        
        y++;
        
        // Ligne séparatrice (sauf après la dernière ligne)
        if (ligne < CARTE_HAUTEUR - 1) {
            deplacer_curseur_fenetre(win, y, 0);
            afficher_texte_fenetre(win, "├─────────┼─────────┼─────────┼─────────┼────────────┤");
            y++;
        }
    }
    
    // Ligne inférieure du tableau
    deplacer_curseur_fenetre(win, y, 0);
    afficher_texte_fenetre(win, "└─────────┴─────────┴─────────┴─────────┴────────────┘");
}

void afficher_infos_zone(InterfaceCarte *interface, const ZoneCarte *zone) {
    Fenetre *win = interface->info_win;
    fenetre_nettoyer(win);
    
    centrer_texte(win, 0, "INFORMATIONS DE LA ZONE");
    
    int y = 2;
    
    if (zone->decouverte) {
        // Nom et emoji
        deplacer_curseur_fenetre(win, y, 2);
        afficher_texte_fenetre(win, "Zone: ");
        afficher_texte_fenetre(win, zone->emoji);
        afficher_texte_fenetre(win, " ");
        afficher_texte_fenetre(win, zone->nom);
        y++;
        
        // Description
        if (strlen(zone->description) > 0) {
            deplacer_curseur_fenetre(win, y, 2);
            afficher_texte_fenetre(win, "Description: ");
            afficher_texte_fenetre(win, zone->description);
            y++;
        }
        
        // Ennemis
        if (zone->nb_ennemis > 0) {
            deplacer_curseur_fenetre(win, y, 2);
            char ennemis[32];
            snprintf(ennemis, sizeof(ennemis), "Ennemis présents: %d", zone->nb_ennemis);
            afficher_texte_fenetre(win, ennemis);
            y++;
        } else {
            deplacer_curseur_fenetre(win, y, 2);
            afficher_texte_fenetre(win, "Aucun ennemi dans cette zone");
            y++;
        }
        
        // Profondeur
        deplacer_curseur_fenetre(win, y, 2);
        char prof[32];
        snprintf(prof, sizeof(prof), "Profondeur: %dm", zone->profondeur);
        afficher_texte_fenetre(win, prof);
        y++;
        
    } else {
        centrer_texte(win, 4, "ZONE NON EXPLORÉE");
        centrer_texte(win, 5, "Déplacez-vous vers cette zone pour la découvrir");
    }
}

void afficher_commandes(InterfaceCarte *interface) {
    Fenetre *win = interface->commande_win;
    fenetre_nettoyer(win);
    
    centrer_texte(win, 0, "COMMANDES DE NAVIGATION");
    
    deplacer_curseur_fenetre(win, 2, 2);
    afficher_texte_fenetre(win, "Z: Haut | S: Bas | Q: Gauche | D: Droite");
    
    deplacer_curseur_fenetre(win, 3, 2);
    afficher_texte_fenetre(win, "E: Explorer/Interagir | R: Retour au menu");
}

void mettre_a_jour_interface_carte(InterfaceCarte *interface, const CarteOcean *carte) {
    afficher_carte(interface, carte);
    
    ZoneCarte *zone_actuelle = get_zone_actuelle((CarteOcean*)carte);
    afficher_infos_zone(interface, zone_actuelle);
    
    afficher_commandes(interface);
    
    // Rafraîchir l'affichage global
    // Note: Vous devrez appeler gf_rendu() depuis votre main
}

// Ajoute ces fonctions :

void cacher_interface_carte(InterfaceCarte *interface) {
    if (!interface) return;
    
    interface->visible = false;
    
    // Cacher les fenêtres individuelles
    if (interface->carte_win) interface->carte_win->visible = false;
    if (interface->info_win) interface->info_win->visible = false;
    if (interface->commande_win) interface->commande_win->visible = false;
}

void montrer_interface_carte(InterfaceCarte *interface) {
    if (!interface) return;
    
    interface->visible = true;
    
    // Montrer les fenêtres individuelles
    if (interface->carte_win) interface->carte_win->visible = true;
    if (interface->info_win) interface->info_win->visible = true;
    if (interface->commande_win) interface->commande_win->visible = true;
}

// Modifie la fonction creer_interface_carte pour initialiser la visibilité :


void detruire_interface_carte(InterfaceCarte *interface) {
    if (!interface) return;
    
    // Note: Les fenêtres sont détruites par le GestionFenetre
    free(interface);
}