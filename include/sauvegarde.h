#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "joueur.h"
#include "creatures.h"
#include "combat.h"  

typedef struct {
    Plongeur *plongeur;
    Combat_plongeur *combat_plongeur;
    char equipement[2][32];
    int nv_zone;
    char nom_zone[32];
    CreatureMarine *creatures;
    int nb_creatures;
} SauvegardeJeu;

int extraire_valeur_numerique(const char *json, const char *cle);
void extraire_valeur_chaine(const char *json, const char *cle, char *buffer, size_t taille);
int compter_creatures(const char *json);
int charger_creatures(const char* nom_fichier, CreatureMarine** creatures, int* nb_creatures);
SauvegardeJeu* charger_sauvegarde_complete(const char* nom_fichier);
void liberer_sauvegarde(SauvegardeJeu *sauvegarde);

#endif