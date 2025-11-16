#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/competences.h"


GestionnaireCompetences* initialiser_competences(Plongeur *joueur) {
    if (joueur == NULL) {
        printf("Erreur: pointeur joueur invalide\n");
        return NULL;
    }
    
    GestionnaireCompetences *gc = (GestionnaireCompetences*)malloc(sizeof(GestionnaireCompetences));
    if (gc == NULL) {
        printf("Erreur allocation memoire\n");
        return NULL;
    }
    
    gc->joueur_ref = joueur;
    gc->nb_competences = 4;
    
    // COMPETENCE 1: APNEE PROLONGEE
    gc->competences[0].id = APNEE_PROLONGEE;
        strncpy(gc->competences[0].nom, "Apnee Prolongee", sizeof(gc->competences[0].nom) - 1);
    gc->competences[0].cout_oxygene = 0;
    gc->competences[0].degats_min = 0;
    gc->competences[0].degats_max = 0;
    gc->competences[0].cooldown_max = 4;
    gc->competences[0].cooldown_actuel = 0;
        
    //  COMPETENCE 2: DECHARGE ELECTRIQUE 
    gc->competences[1].id = DECHARGE_ELECTRIQUE;
        strncpy(gc->competences[1].nom, "Decharge Electrique", sizeof(gc->competences[1].nom) - 1);
    gc->competences[1].cout_oxygene = 18;
    gc->competences[1].degats_min = 20;
    gc->competences[1].degats_max = 30;
    gc->competences[1].cooldown_max = 0;
    gc->competences[1].cooldown_actuel = 0;
    
    // COMPETENCE 3: COMMUNICATION MARINE 
    gc->competences[2].id = COMMUNICATION_MARINE;
        strncpy(gc->competences[2].nom, "Communication Marine", sizeof(gc->competences[2].nom) - 1);
    gc->competences[2].cout_oxygene = 8;
    gc->competences[2].degats_min = 0;
    gc->competences[2].degats_max = 0;
    gc->competences[2].cooldown_max = 0;
    gc->competences[2].cooldown_actuel = 0;
    
    // COMPETENCE 4: TOURBILLON AQUATIQUE 
    gc->competences[3].id = TOURBILLON_AQUATIQUE;
        strncpy(gc->competences[3].nom, "Tourbillon Aquatique", sizeof(gc->competences[3].nom) - 1);
    gc->competences[3].cout_oxygene = 22;
    gc->competences[3].degats_min = 0;
    gc->competences[3].degats_max = 0;
    gc->competences[3].cooldown_max = 0;
    gc->competences[3].cooldown_actuel = 0;
    
    printf("Competences initialisées!\n");
    return gc;
}
Competence* obtenir_competence(GestionnaireCompetences *gc, IdCompetence id) {
    if (gc == NULL) return NULL;
    
    for (int i = 0; i < gc->nb_competences; i++) {
        if (gc->competences[i].id == id) {
            return &gc->competences[i];
        }
    }
    return NULL;
}

bool competence_utilisable(GestionnaireCompetences *gc, IdCompetence id) {
    if (gc == NULL || gc->joueur_ref == NULL) return false;
    
    Competence *comp = obtenir_competence(gc, id);
    if (comp == NULL) return false;
    if (comp->cooldown_actuel > 0) {
        printf("Cette competence est en cooldown (%d tours)\n", comp->cooldown_actuel);
        return false;
    }
    
    // Verifier l'oxygene disponible
    if (gc->joueur_ref->niveau_oxygene < comp->cout_oxygene) {
        printf("Oxygene insuffisant! Cout: %d, Disponible: %d\n", 
               comp->cout_oxygene, gc->joueur_ref->niveau_oxygene);
        return false;
    }
    
    return true;
}

int utiliser_competence(GestionnaireCompetences *gc, IdCompetence id, 
                        CreatureMarine *cibles[], int nb_cibles) {
    
    if (!competence_utilisable(gc, id)) {
        return -1;
    }
    
    Competence *comp = obtenir_competence(gc, id);
    if (comp == NULL) return -1;
    
    // Deduire l'oxygene
    gc->joueur_ref->niveau_oxygene -= comp->cout_oxygene;
    if (gc->joueur_ref->niveau_oxygene < 0) {
        gc->joueur_ref->niveau_oxygene = 0;
    }
    
    // Mettre le cooldown en place
    comp->cooldown_actuel = comp->cooldown_max;
    
    printf("\nUTILISATION DE COMPETENCE\n");
    printf("=====================================\n");
    printf("Competence: %s\n", comp->nom);
    printf("Cout O2: -%d (O2 restant: %d)\n", comp->cout_oxygene, gc->joueur_ref->niveau_oxygene);
    
    // Application des effets selon la competence
    if (id == APNEE_PROLONGEE) {
        gc->joueur_ref->niveau_oxygene += 20;
        if (gc->joueur_ref->niveau_oxygene > gc->joueur_ref->niveau_oxygene_max) {
            gc->joueur_ref->niveau_oxygene = gc->joueur_ref->niveau_oxygene_max;
        }
        printf("Effet: +20 oxygene (Total: %d/%d)\n", 
               gc->joueur_ref->niveau_oxygene, gc->joueur_ref->niveau_oxygene_max);
    }
    else if (id == DECHARGE_ELECTRIQUE) {
        int degats = comp->degats_min + rand() % (comp->degats_max - comp->degats_min + 1);
        printf("Effet: Degats zone %d appliques a %d creature(s)\n", 
               degats, nb_cibles);
        
        for (int i = 0; i < nb_cibles && cibles[i] != NULL; i++) {
            if (cibles[i]->est_vivant) {
                cibles[i]->points_de_vie_actuels -= degats;
                if (cibles[i]->points_de_vie_actuels < 0) {
                    cibles[i]->points_de_vie_actuels = 0;
                }
                printf("  - %s: -%d PV (PV restants: %d)\n", 
                       cibles[i]->nom, degats, cibles[i]->points_de_vie_actuels);
            }
        }
    }
    else if (id == COMMUNICATION_MARINE) {
        if (nb_cibles > 0 && cibles[0] != NULL) {
            printf("Effet: %s pacifiee (1 tour)\n", cibles[0]->nom);
        }
    }
    else if (id == TOURBILLON_AQUATIQUE) {
        printf("Effet: Toutes les creatures melangees et -2 vitesse\n");
        for (int i = 0; i < nb_cibles && cibles[i] != NULL; i++) {
            if (cibles[i]->est_vivant) {
                cibles[i]->vitesse -= 2;
                printf("  - %s: -2 vitesse (vitesse: %d)\n", cibles[i]->nom, cibles[i]->vitesse);
            }
        }
    }
    
    printf("=====================================\n");
    return 0;
}

void reduire_cooldowns(GestionnaireCompetences *gc) {
    if (gc == NULL) return;
    
    for (int i = 0; i < gc->nb_competences; i++) {
        if (gc->competences[i].cooldown_actuel > 0) {
            gc->competences[i].cooldown_actuel--;
            if (gc->competences[i].cooldown_actuel == 0) {
                printf("Competence %s est a nouveau disponible!\n", gc->competences[i].nom);
            }
        }
    }
}
void afficher_competences_disponibles(GestionnaireCompetences *gc) {
    if (gc == NULL || gc->joueur_ref == NULL) return;
    
    printf("\n");
    printf("====== COMPETENCES MARINES ======\n");
    printf("\n");
    
    for (int i = 0; i < gc->nb_competences; i++) {
        Competence *comp = &gc->competences[i];
        bool utilisable = competence_utilisable(gc, comp->id);
        afficher_details_competence(comp, utilisable);
    }
    
    printf("\nOxygene actuel: %d/%d\n", 
           gc->joueur_ref->niveau_oxygene, gc->joueur_ref->niveau_oxygene_max);
}

void afficher_details_competence(Competence *comp, bool utilisable) {
    if (comp == NULL) return;
    
    char symbole[10];
    
    if (utilisable) {
        strcpy(symbole, "OK");
    } else {
        strcpy(symbole, "X");
    }
    
    printf("  [%d] %s - %s\n", comp->id, symbole, comp->nom);
    printf("      Cout O2: %d\n", comp->cout_oxygene);
    
    if (comp->cooldown_actuel > 0) {
        printf("      Cooldown: %d tour(s)\n", comp->cooldown_actuel);
    }
    printf("\n");
}

void afficher_menu_competences_format(GestionnaireCompetences *gc) {
    if (gc == NULL) return;
    
    printf("\n");
    printf("                   COMPETENCES MARINES\n");
    printf("======================================================================\n");
    printf("======================================================================\n");
    printf("\n");
    printf("|| [1] Apnee Prolongee       [2] Decharge Electrique       ||\n");
    printf("||     Cout: 0 oxygene            Cout: 18 oxygene         ||\n");
    printf("||     +20 oxygene                Degats zone: 20-30        ||\n");
    printf("||     Cooldown: 4 tours          Toutes les creatures     ||\n");
    printf("||\n");
    printf("|| [3] Communication Marine  [4] Tourbillon Aquatique      ||\n");
    printf("||     Cout: 8 oxygene            Cout: 22 oxygene         ||\n");
    printf("||     1 creature pacifiee        Melange ennemis          ||\n");
    printf("||     Duree: 1 tour              -2 vitesse ennemis       ||\n");
    printf("\n");
    printf("======================================================================\n");
    printf("======================================================================\n");
    printf("\n");
        printf("Oxygene actuel: %d/%d\n\n",
            gc->joueur_ref->niveau_oxygene, gc->joueur_ref->niveau_oxygene_max);
}
void recuperation_oxygene_naturelle(Plongeur *joueur, bool en_combat) {
    if (joueur == NULL) return;
    
    int recuperation = en_combat ? 8 : 15;
    int oxygene_avant = joueur->niveau_oxygene;
    
    joueur->niveau_oxygene += recuperation;
    if (joueur->niveau_oxygene > joueur->niveau_oxygene_max) {
        joueur->niveau_oxygene = joueur->niveau_oxygene_max;
    }
    
    if (joueur->niveau_oxygene > oxygene_avant) {
        printf("Recuperation naturelle: +%d O2 (%d -> %d)\n", 
               joueur->niveau_oxygene - oxygene_avant, oxygene_avant, joueur->niveau_oxygene);
    }
}

int utiliser_capsule_oxygene(Plongeur *joueur) {
    if (joueur == NULL) return -1;
    
    if (joueur->niveau_oxygene == joueur->niveau_oxygene_max) {
        printf("Oxygene deja au maximum!\n");
        return -1;
    }
    
    int oxygene_avant = joueur->niveau_oxygene;
    joueur->niveau_oxygene += 40;
    
    if (joueur->niveau_oxygene > joueur->niveau_oxygene_max) {
        joueur->niveau_oxygene = joueur->niveau_oxygene_max;
    }
    
    printf("Capsule O2 utilisee: +%d O2 (%d -> %d)\n", 
           joueur->niveau_oxygene - oxygene_avant, oxygene_avant, joueur->niveau_oxygene);
    return 0;
}

bool oxygene_est_critique(Plongeur *joueur) {
    if (joueur == NULL) return false;
    return joueur->niveau_oxygene < OXYGENE_CRITIQUE;
}

void liberer_competences(GestionnaireCompetences *gc) {
    if (gc != NULL) {
        free(gc);
    }
}
