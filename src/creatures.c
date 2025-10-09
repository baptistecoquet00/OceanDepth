#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/creatures.h"

void generer_creatures(CreatureMarine *tableau, int *nb_creatures, int profondeur)
{
    if (!tableau || !nb_creatures)
        return;
    srand((unsigned)time(NULL)); // permet d'avoir les creatures aléatoirment

    for (int i = 0; i < *nb_creatures; i++)
    {
        CreatureMarine *creature = &tableau[i];
        creature->id = i + 1;

        switch (i)
        {
        case KRAKEN:
            strcpy(creature->nom, "Kraken");
            creature->points_de_vie_max = 120;
            creature->points_de_vie_actuels = 120;
            creature->attaque_minimale = 25;
            creature->attaque_maximale = 40;
            creature->defense = 12;
            creature->vitesse = 4;
            strcpy(creature->effet_special, "rapide");
            creature->est_vivant = 1;
            break;

        case REQUIN:
            strcpy(creature->nom, "Requin");
            creature->points_de_vie_max = 100;
            creature->points_de_vie_actuels = 100;
            creature->attaque_minimale = 15;
            creature->attaque_maximale = 25;
            creature->defense = 8;
            creature->vitesse = 20;
            strcpy(creature->effet_special, "attaques multiples");
            creature->est_vivant = 1;
            break;

        case MEDUSE:
            strcpy(creature->nom, "Méduse");
            creature->points_de_vie_max = 35;
            creature->points_de_vie_actuels = 35;
            creature->attaque_minimale = 3;
            creature->attaque_maximale = 8;              
            creature->defense = 2;                        
            creature->vitesse = 5;                       
            strcpy(creature->effet_special, "paralysie"); // ses filaments paralysent
            creature->est_vivant = 1;
            break;

        case POISSON_EPEE:
            strcpy(creature->nom, "Poisson Épée");
            creature->points_de_vie_max = 50;
            creature->points_de_vie_actuels = 50;
            creature->attaque_minimale = 6;
            creature->attaque_maximale = 12;          
            creature->defense = 4;                   
            creature->vitesse = 9;                    
            strcpy(creature->effet_special, "aucun"); // pas de pouvoir magique
            creature->est_vivant = 1;
            break;

        case SERPENT_DE_MER:
            strcpy(creature->nom, "Serpent de mer");
            creature->points_de_vie_max = 70;
            creature->points_de_vie_actuels = 70;
            creature->attaque_minimale = 10;
            creature->attaque_maximale = 18;
            creature->defense = 6;
            creature->vitesse = 8;
            strcpy(creature->effet_special, "poison"); // morsure empoisonnée
            creature->est_vivant = 1;
            break;

        default:
            strcpy(creature->nom, "Créature inconnue");
            creature->points_de_vie_max = 10;
            creature->points_de_vie_actuels = 10;
            creature->attaque_minimale = 1;
            creature->attaque_maximale = 2;
            creature->defense = 1;
            creature->vitesse = 1;
            strcpy(creature->effet_special, "aucun");
            creature->est_vivant = 1;

            break;
        }
    }
}