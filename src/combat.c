#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "combat.h"
#include "inventaire.h"

/* helper: calc attacks allowed from fatigue */
int calcul_attacks_from_fatigue(int fatigue) {
    if (fatigue <= 1) return 3;
    if (fatigue <= 3) return 2;
    return 1;
}

/* Attaque des créatures selon vitesse décroissante, avec effets spéciaux */
void attaque_creatures(Plongeur *joueur, CreatureMarine creatures[], int nb_creatures) {
    // tri simple par vitesse (selection sort) - plus rapide agit d'abord
    for (int i = 0; i < nb_creatures - 1; i++) {
        int imax = i;
        for (int j = i+1; j < nb_creatures; j++) {
            if (creatures[j].vitesse > creatures[imax].vitesse) imax = j;
        }
        if (imax != i) {
            CreatureMarine tmp = creatures[i];
            creatures[i] = creatures[imax];
            creatures[imax] = tmp;
        }
    }

    for (int i = 0; i < nb_creatures; i++) {
        if (!creatures[i].est_vivant) continue;

        int nb_attaques = 1;
        if (strcmp(creatures[i].nom, "Kraken") == 0) nb_attaques = 2;

        for (int a = 0; a < nb_attaques; a++) {
            // attaque de la créature
            int atk = rand() % (creatures[i].attaque_maximale - creatures[i].attaque_minimale + 1)
                      + creatures[i].attaque_minimale;

            // effets spéciaux
            int def_joueur = joueur->defense;
            if (strcmp(creatures[i].nom, "Poisson-Épée") == 0) {
                def_joueur -= 2;
                if (def_joueur < 0) def_joueur = 0;
            }
            if (strcmp(creatures[i].nom, "Requin") == 0) {
                if (creatures[i].points_de_vie_actuels < creatures[i].points_de_vie_max / 2) {
                    atk = (int)(atk * 1.3); // +30%
                }
            }
            int degats = atk - def_joueur;
            if (degats < 1) degats = 1;

            // appliquer réduction si joueur a carapace passive ? (non implémenté)
            // infliger dégâts
            joueur->points_de_vie -= degats;
            // stress -> perte O2 1..2
            int perte_o2 = 1 + rand() % 2;
            joueur->niveau_oxygene -= perte_o2;

            printf("%s vous attaque et inflige %d dégâts ! (Oxygène -%d)\n", creatures[i].nom, degats, perte_o2);

            // effets persistants
            if (strcmp(creatures[i].nom, "Méduse") == 0) {
                printf("⚡ La Méduse a infligé une paralysie : -1 attaque au prochain tour.\n");
                joueur->paralyse = 1;
            }

            if (joueur->niveau_oxygene <= 0) {
                joueur->niveau_oxygene = 0;
                printf("⚠️ Oxygène épuisé ! Vous perdez 5 PV d'asphyxie.\n");
                joueur->points_de_vie -= 5;
            }

            if (joueur->points_de_vie <= 0) {
                joueur->points_de_vie = 0;
                printf("💀 Vous avez été vaincu...\n");
                return;
            }
        }
    }

    // récupération fatigue + calcul attaques_restantes
    if (joueur->niveau_fatigue > 0) joueur->niveau_fatigue--; // récupération d'1 niveau
    joueur->attaques_restantes = calcul_attacks_from_fatigue(joueur->niveau_fatigue);
    if (joueur->paralyse > 0) joueur->paralyse--; // dure seulement 1 tour ici
}
