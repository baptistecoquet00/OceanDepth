#include <stdio.h>
#include <stdlib.h>
#include "include/creatures.h"
#include "include/combat.h"

Combat_plongeur combat_calcul_degats(int attaque_joueur, int bonus_arme){
    CreatureMarine creature;
    int defense_creature = creature.defense;
    int degats = (attaque_joueur + bonus_arme) - defense_creature;
    if(degats < 1) degats = 1; 
}

Combat_plongeur combat_calcul_fatigue(Combat_plongeur plongeur_combat,Fatigue niveau_fatigue){
    int calcul_fatigue = plongeur_combat.fatigue.niveau_fatigue; 
    if(calcul_fatigue >= FATIGUE_NV_ZERO || calcul_fatigue <= FATIGUE_NV_UN){
        plongeur_combat.nb_attaque_par_tour = 3;
    }else if(calcul_fatigue >= FATIGUE_NV_DEUX || calcul_fatigue <= FATIGUE_NV_TROIS){
        plongeur_combat.nb_attaque_par_tour = 2;
    }else if(calcul_fatigue >=FATIGUE_NV_QUATRE || calcul_fatigue <=FATIGUE_NV_CINQ){
        plongeur_combat.nb_attaque_par_tour = 1;
    }

    return plongeur_combat;
}

Combat_plongeur combat_gestion_vie(int points_de_vie){
    // TODO
}

Combat_plongeur combat_gestion_oxygene(Combat_plongeur plongeur_combat,Plongeur plongeur){
    int oxygene;
    // potentielement transformé en fonction inline
    if(plongeur_combat.attaque_normle){
        plongeur_combat.fatigue.niveau_oxygene -= 2;
        oxygene = plongeur_combat.fatigue.niveau_oxygene;
    }
    // potentielement transformé en fonction inline
    if(plongeur_combat.competence_special){
        plongeur_combat.fatigue.niveau_oxygene -= 5;
        oxygene = plongeur_combat.fatigue.niveau_oxygene;
    }
    // potentielement transformé en fonction inline
    if(oxygene<10){
        printf("ALERTE ROUGE\n");
    }
    while (oxygene == 0)
    {
        int pv_plongeur = plongeur_combat.fatigue.points_de_vie;
        pv_plongeur -=5;
        if(pv_plongeur <= 0){
            plongeur.points_de_vie = pv_plongeur;
            est_mort(plongeur);
            break;
        }
    }
    return plongeur_combat;
}

