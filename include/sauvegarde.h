#ifndef SAUVEGARDER_H
#define SAUVEGARDER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "joueur.h"
#include "creatures.h"
#include "combat.h" 
#include "charger.h"

int sauvegarder_plongeur(const char* nom_fichier, Plongeur* plongeur);
int sauvegarder_creatures(const char* nom_fichier, CreatureMarine* creatures, int nb_creatures);
int sauvegarder_jeu_complet(const char* nom_fichier, Plongeur* plongeur, CreatureMarine* creatures, int nb_creatures);