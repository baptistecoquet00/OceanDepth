#include <stdio.h>
#include <stdlib.h>
#include "../../include/joueur.h"
#include "../../include/creatures.h"
#include "../../include/combat.h"
#include "../../include/interface_combat.h"

Interface_combat nouvelle_interface_combat();
void afficher_interface_combat_exemple(Combat_plongeur* plongeur_combat){

    printf("╔════════════════════ COMBAT SOUS-MARIN ════════════════════╗\n");
    printf("║ Vous attaquez le Requin-Tigre avec votre Harpon Rouillé   ║\n");
    printf("║                                                           ║\n");
    printf("║                      PLONGEUR VS REQUIN                   ║\n");
    printf("║                       ♠        🦈                         ║\n");
    printf("║ 	             ════════▶ ◀════════                    ║\n");
    printf("║ Dégâts infligés: %d points                                 ║\n",plongeur_combat->attaque_normale);
    printf("║ Le Requin-Tigre riposte! Vous perdez 18 points de vie     ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
}

void afficher_interface_combat(){
   printf("╔");
    for (int i = 0; i < LONGEUR_INTERFACE - 2; i++){ 
        if(i == LONGEUR_INTERFACE/2){
            printf(" COMBAT SOUS-MARIN ");
        }
        printf("═");
    }
    printf("╗\n");

    
    // Corps du cadre
    for (int i = 0; i < LARGEUR_INTERFACE - 2; i++) {
        printf("║");
        for (int j = 0; j < LONGEUR_INTERFACE + 19 - 2; j++){ 
            printf(" ");
            if(j == LARGEUR_INTERFACE/2 && i == LONGEUR_INTERFACE/2) printf("════════▶ ◀════════");
        }
        printf("║\n");
    }
    
    // Ligne du bas
    printf("╚");
    for (int i = 0; i < LONGEUR_INTERFACE + 19 - 2; i++) printf("═");
    printf("╝\n");
}