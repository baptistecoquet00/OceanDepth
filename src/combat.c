#include <stdio.h>
#include <stdlib.h>
#include "include/creatures.h"
#include "include/combat.h"

Plongeur combat_calcul_degats(int attaque_joueur, int bonus_arme){
    CreatureMarine creature;
    int defense_creature = creature.defense;
    int degats = (attaque_joueur + bonus_arme) - defense_creature;
    if(degats < 1) degats = 1; 
}

Plongeur combat_calcul_fatigue(Plongeur* plongeur,int niveau_fatigue){
    
    // TODO
    return plongeur->niveau_fatigue;

}

Plongeur combat_gestion_vie(int points_de_vie){
    // TODO
}