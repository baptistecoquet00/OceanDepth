#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "../include/joueur.h"
#include "../include/creatures.h"
#include "../include/combat.h"
#include "../include/inventaire.h"
#include "../include/sauvegarde.h"
#include "../include/carte.h"

int main() {
    srand((unsigned)time(NULL));


    Plongeur joueur;
    init_plongeur(&joueur);


   
    CreatureMarine creatures[MAX_CREATURES];
    int nb_creatures = 0;
    init_creatures_random(creatures, &nb_creatures, carte.profondeur);

    printf("=== OCEANDEPTHS - TEST PARTIE ATTAQUE DES CRÉATURES ===\n");
    printf("Profondeur : %dm  | Zone : %s\n", carte.profondeur, carte.zone);
    printf("Plongeur : PV %d/%d  Oxygène %d/%d  Fatigue %d  Perles %d\n\n",
           joueur.points_de_vie, joueur.points_de_vie_max,
           joueur.niveau_oxygene, joueur.niveau_oxygene_max,
           joueur.niveau_fatigue, joueur.perles);

    printf("Créatures rencontrées : %d\n", nb_creatures);
    for (int i = 0; i < nb_creatures; i++) {
        printf(" - %s (PV %d/%d) ATK %d-%d DEF %d VIT %d EFFET %s\n",
               creatures[i].nom, creatures[i].points_de_vie_actuels, creatures[i].points_de_vie_max,
               creatures[i].attaque_minimale, creatures[i].attaque_maximale,
               creatures[i].defense, creatures[i].vitesse, creatures[i].effet_special);
    }

    printf("\n--- Tour de combat des créatures ---\n");
    attaque_creatures(&joueur, creatures, nb_creatures);

    printf("\nÉtat après attaque : PV %d/%d | Oxygène %d/%d | Fatigue %d\n",
           joueur.points_de_vie, joueur.points_de_vie_max,
           joueur.niveau_oxygene, joueur.niveau_oxygene_max,
           joueur.niveau_fatigue);

    // test sauvegarde
    if (save_game("saves/save_test.txt", &joueur, &inv, carte.profondeur, creatures, nb_creatures)) {
        printf("\nSauvegarde effectuée dans saves/save_test.txt\n");
    } else {
        printf("\nErreur sauvegarde (créer dossier saves/ si absent)\n");
    }

    printf("\n=== FIN DU TEST ===\n");
    return 0;
}
