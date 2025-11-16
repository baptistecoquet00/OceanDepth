#ifndef COMPETENCES_H
#define COMPETENCES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "joueur.h"
#include "creatures.h"

typedef enum {
    APNEE_PROLONGEE = 1,
    DECHARGE_ELECTRIQUE = 2,
    COMMUNICATION_MARINE = 3,
    TOURBILLON_AQUATIQUE = 4
} IdCompetence;

typedef struct {
    IdCompetence id;
    char nom[50];
    int cout_oxygene;
    int degats_min;
    int degats_max;
    int cooldown_max;
    int cooldown_actuel;
} Competence;

typedef struct {
    Competence competences[4];
    int nb_competences;
    Plongeur *joueur_ref;
} GestionnaireCompetences;

GestionnaireCompetences* initialiser_competences(Plongeur *joueur);
Competence* obtenir_competence(GestionnaireCompetences *gc, IdCompetence id);

bool competence_utilisable(GestionnaireCompetences *gc, IdCompetence id);

int utiliser_competence(GestionnaireCompetences *gc, IdCompetence id, 
                        CreatureMarine *cibles[], int nb_cibles);

void reduire_cooldowns(GestionnaireCompetences *gc);
void afficher_competences_disponibles(GestionnaireCompetences *gc);
void afficher_details_competence(Competence *comp, bool utilisable);
void afficher_menu_competences_format(GestionnaireCompetences *gc);

void recuperation_oxygene_naturelle(Plongeur *joueur, bool en_combat);
int utiliser_capsule_oxygene(Plongeur *joueur);
bool oxygene_est_critique(Plongeur *joueur);
void liberer_competences(GestionnaireCompetences *gc);

#endif