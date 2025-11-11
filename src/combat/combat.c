#include <stdio.h>
#include <stdlib.h>

#include "../../include/gm.h"
#include "../../include/creatures.h"
#include "../../include/tour_par_tour.h"
#include "../../include/combat.h"




Combat_plongeur* nouveau_combat_plongeur(Plongeur* plongeur){
    Combat_plongeur* plongeur_combat = (Combat_plongeur*)malloc(sizeof(Combat_plongeur));
    plongeur_combat->nb_attaque_par_tour = NB_MAXIMUM_ATTAQUE_PAR_TOUR;
    plongeur_combat->attaque_normale = 0;
    plongeur_combat->attaque_special = 0;
    plongeur_combat->competence_special = 0;
    plongeur_combat->gestion_fatigue_vie = plongeur;
    return plongeur_combat;
}

void afficher_combat_plongeur(Combat_plongeur* combat_plongeur){
    printf("nombre d'attaque par tour : %d\n",combat_plongeur->nb_attaque_par_tour);
    printf("Attaque normale : %d\n",combat_plongeur->attaque_normale);
    printf("Attque spéciale : %d\n",combat_plongeur->attaque_special);
    printf("Competence spéciale : %d\n",combat_plongeur->competence_special);
    printf("========================\n");
    afficher_plongeur(combat_plongeur->gestion_fatigue_vie);
   
}

int combat_calcul_degats(int attaque_joueur, int bonus_arme){
    //CreatureMarine creature = nouvelle_creature();
    CreatureMarine creature;
    creature.defense = 10;
    int defense_creature = creature.defense;
    int degats = (attaque_joueur + bonus_arme) - defense_creature;
    if(degats < 1) degats = 1; 
    return degats;
}

void combat_plongeur_calcul_fatigue(Combat_plongeur* plongeur_combat){
    int calcul_fatigue = plongeur_combat->gestion_fatigue_vie->niveau_fatigue; 
    
    if(calcul_fatigue >= FATIGUE_NV_ZERO && calcul_fatigue <= FATIGUE_NV_UN)
    {
        plongeur_combat->nb_attaque_par_tour = NB_MAXIMUM_ATTAQUE_PAR_TOUR;
    }
    else if(calcul_fatigue >= FATIGUE_NV_DEUX && calcul_fatigue <= FATIGUE_NV_TROIS)
    {
        plongeur_combat->nb_attaque_par_tour = NB_MOYEN_ATTAQUE_PAR_TOUR;
    }
    else if(calcul_fatigue >= FATIGUE_NV_QUATRE && calcul_fatigue <=FATIGUE_NV_CINQ)
    {
        plongeur_combat->nb_attaque_par_tour = NB_BAS_ATTAQUE_PAR_TOUR;
    }

    return;
    //return plongeur_combat;
}

void combat_plongeur_gestion_vie(Combat_plongeur* plongeur_combat, CreatureMarine* creature){
    
    if(creature->attaque_maximale) 
    {
        plongeur_combat->gestion_fatigue_vie->points_de_vie -= creature->attaque_maximale;
    }
    
    if(creature->attaque_minimale)
    {
        plongeur_combat->gestion_fatigue_vie->points_de_vie -= creature->attaque_minimale;
    }

    if(est_mort(plongeur_combat->gestion_fatigue_vie)==0){
        return;
        //return plongeur_combat;    
    }
    return;
    //return plongeur_combat;
}

inline int attaque_normal(Combat_plongeur* plongeur_combat){
    plongeur_combat->gestion_fatigue_vie->niveau_oxygene -= 2;
    return plongeur_combat->gestion_fatigue_vie->niveau_oxygene;
}

inline int competence_speciale(Combat_plongeur* plongeur_combat){
    plongeur_combat->gestion_fatigue_vie->niveau_oxygene -= 5;
    return plongeur_combat->gestion_fatigue_vie->niveau_oxygene;
}

void combat_plongeur_gestion_oxygene(Combat_plongeur* plongeur_combat){

    if(plongeur_combat->attaque_normale){
        plongeur_combat->gestion_fatigue_vie->niveau_oxygene += attaque_normal(plongeur_combat);
    }

    if(plongeur_combat->competence_special){
        plongeur_combat->gestion_fatigue_vie->niveau_oxygene += competence_speciale(plongeur_combat);
    }
    
    plongeur_combat->gestion_fatigue_vie->niveau_oxygene = oxygene_vide(plongeur_combat->gestion_fatigue_vie);
    //oxygene_critique(plongeur_combat.gestion_fatigue_vie);

    if(plongeur_combat->gestion_fatigue_vie->niveau_oxygene <= 0)
    {
        plongeur_combat->gestion_fatigue_vie->points_de_vie -=5;
        //printf("Point de vie avant la mort : %d\n",plongeur_combat.gestion_fatigue_vie.points_de_vie);
        
        if(est_mort(plongeur_combat->gestion_fatigue_vie)==0){
           return;
        }
    }
    return;

    //afficher_plongeur(plongeur_combat.gestion_fatigue_vie);
    //printf("Niveau d'oxygène : %d\n",plongeur_combat.gestion_fatigue_vie.niveau_oxygene);
    //return plongeur_combat;
}

void Systeme_combat(Combat_plongeur *plongeur_combat, GestionFenetre *gf /*,Arene *jeu_arene*/) {
    TourJoueur tour;
    nouveau_tour(&tour);

    // Création des fenêtres de combat
    Fenetre *combat_win = gf_creer_fenetre(gf, 5, 3, 70, 15, "COMBAT SOUS-MARIN");
    Fenetre *actions_win = gf_creer_fenetre(gf, 5, 19, 70, 4, "ACTIONS");
    
    // Initialisation de l'ennemi avec ta structure
    CreatureMarine ennemi = {
        .id = 1,
        .nom = "Requin-Tigre",
        .points_de_vie_max = 100,
        .points_de_vie_actuels = 100,
        .attaque_minimale = 10,
        .attaque_maximale = 20,
        .defense = 8,
        .vitesse = 15,
        .effet_special = "saignement",
        .est_vivant = 1
    };

    bool combat_actif = true;

    while (combat_actif) {
        // Effacer et recréer les fenêtres
        gf->nb_fenetre = 0;
        combat_win = gf_creer_fenetre(gf, 5, 3, 70, 15, "COMBAT SOUS-MARIN");
        actions_win = gf_creer_fenetre(gf, 5, 19, 70, 4, "ACTIONS");
        
        // Interface de base
        fenetre_dessiner_bordure(combat_win);
        fenetre_dessiner_bordure(actions_win);
        
        afficher_fenetre(combat_win, 25, 3, "PLONGEUR VS REQUIN");
        afficher_fenetre(combat_win, 28, 4, "♠        🦈");
        afficher_fenetre(combat_win, 20, 5, "---------> <---------");
        
        // Afficher les stats avec ta structure CreatureMarine
        afficher_fenetref(combat_win, 2, 7, "PV Joueur: %d/%d", 
                         plongeur_combat->gestion_fatigue_vie->points_de_vie,
                         plongeur_combat->gestion_fatigue_vie->points_de_vie_max);
        afficher_fenetref(combat_win, 2, 8, "PV %s: %d/%d", 
                         ennemi.nom, ennemi.points_de_vie_actuels, ennemi.points_de_vie_max);
        afficher_fenetref(combat_win, 2, 9, "Effet: %s", ennemi.effet_special);
        
        if (tour_est_au_joueur(&tour)) {
            if (tour_attente_action(&tour)) {
                // ⏳ En attente de l'action du joueur
                afficher_fenetre(actions_win, 2, 1, "CHOISISSEZ UNE ACTION:");
                afficher_fenetre(actions_win, 2, 2, "1. Attaque    2. Competence    3. Defendre");
                afficher_fenetre(actions_win, 2, 3, "4. Objet      5. Fuir");
                
                printf("Tour %d - Joueur: ", tour_numero(&tour));
                int action = getchar() - '0';
                getchar();  // Consommer le \n
                
                if (action >= 1 && action <= 5) {
                    executer_action_joueur(plongeur_combat, &ennemi, action,actions_win);
                    tour_action_effectuee(&tour);
                }
                
            } else {
                // ✅ Le joueur a agi, on passe à l'ennemi
                afficher_fenetre(actions_win, 2, 1, "Action effectuee! Passage a l'ennemi...");
                tour_passer_creature(&tour);
            }
            
        } else {
            // 🦈 Tour de l'ennemi avec CreatureMarine
            afficher_fenetref(actions_win, 2, 1, "Tour du %s...", ennemi.nom);
            
            // Attaque de l'ennemi avec tes stats
            int degats_ennemi = calculer_degats_creature(&ennemi);
            int degats_finals = degats_ennemi; // Simplifié, tu peux ajouter la défense du joueur
            
            plongeur_combat->gestion_fatigue_vie->points_de_vie -= degats_finals;
            
            afficher_fenetref(combat_win, 2, 11, "%s attaque! -%d PV", ennemi.nom, degats_finals);
            
            // Appliquer effet spécial si pertinent
            appliquer_effet_special(&ennemi, plongeur_combat,actions_win);
            
            // Gestion oxygène après l'action
            combat_plongeur_gestion_oxygene(plongeur_combat);
            
            tour_suivant(&tour);
        }
        
        // Vérifier conditions de fin de combat
        if (plongeur_combat->gestion_fatigue_vie->points_de_vie <= 0) {
            afficher_fenetre(actions_win, 2, 1, "VOUS ETES MORT!");
            combat_actif = false;
        } else if (ennemi.points_de_vie_actuels <= 0) {
            ennemi.est_vivant = 0;
            afficher_fenetref(actions_win, 2, 1, "VOUS AVEZ VAINCU LE %s!", ennemi.nom);
            combat_actif = false;
        }
        
        gf_rendu(gf);
        
        if (combat_actif) {
            sleep(2);
        }
    }
}

// Calculer les dégâts d'une créature (aléatoire entre min et max)
int calculer_degats_creature(CreatureMarine *creature) {
    return creature->attaque_minimale + 
           rand() % (creature->attaque_maximale - creature->attaque_minimale + 1);
}

// Appliquer les effets spéciaux
void appliquer_effet_special(CreatureMarine *creature, Combat_plongeur *plongeur,Fenetre *combat_win) {
    if (strcmp(creature->effet_special, "poison") == 0) {
        // Logique poison
        plongeur->gestion_fatigue_vie->points_de_vie -= 3;
        afficher_fenetre(combat_win, 2, 12, "Vous etes empoisonne! -3 PV");
    }
    else if (strcmp(creature->effet_special, "paralysie") == 0) {
        // Logique paralysie (prochain tour sauté)
        afficher_fenetre(combat_win, 2, 12, "Vous etes paralyse!");
    }
    else if (strcmp(creature->effet_special, "saignement") == 0) {
        // Logique saignement
        plongeur->gestion_fatigue_vie->points_de_vie -= 2;
        afficher_fenetre(combat_win, 2, 12, "Vous saignez! -2 PV");
    }
}

// Exécuter l'action du joueur
void executer_action_joueur(Combat_plongeur *plongeur, CreatureMarine *ennemi, int action,Fenetre *actions_win) {
    switch(action) {
        case 1: // Attaque normale
            plongeur->attaque_normale = true;
            int degats_joueur = 8 + rand() % 5; // Exemple: 8-12 dégâts
            ennemi->points_de_vie_actuels -= degats_joueur;
            afficher_fenetref(actions_win, 2, 1, "Attaque! -%d PV au %s", degats_joueur, ennemi->nom);
            break;
            
        case 2: // Compétence spéciale
            plongeur->competence_special = true;
            int degats_speciaux = 15 + rand() % 10; // 15-24 dégâts
            ennemi->points_de_vie_actuels -= degats_speciaux;
            afficher_fenetref(actions_win, 2, 1, "Competence speciale! -%d PV", degats_speciaux);
            break;
            
        case 3: // Défendre
            afficher_fenetre(actions_win, 2, 1, "Vous vous defendez!");
            // Réduit les dégâts du prochain tour
            break;
            
        case 4: // Objet
            afficher_fenetre(actions_win, 2, 1, "Vous utilisez un objet!");
            // Logique d'objet
            break;
            
        case 5: // Fuir
            afficher_fenetre(actions_win, 2, 1, "Vous tentez de fuir...");
            // Logique de fuite
            break;
    }
    
    // Réinitialiser les flags
    plongeur->attaque_normale = false;
    plongeur->competence_special = false;
}

void free_combat_plongeur(Combat_plongeur* plongeur_combat){
    if (plongeur_combat != NULL) {        
        free(plongeur_combat);
    }
}
