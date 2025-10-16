#include <stdio.h>
#include <stdlib.h>
#include "../../include/joueur.h"
#include "../../include/creatures.h"
#include "../../include/combat.h"
#include "../../include/interface_combat.h"

Interface_combat nouvelle_interface_combat();
void afficher_interface_combat_exemple(/*Interface_combat interface,Combat_plongeur plongeur_combat, CreatureMarine creature*/){
    printf("╔════════════════════ COMBAT SOUS-MARIN ════════════════════╗\n");
    printf("║ Vous attaquez le Requin-Tigre avec votre Harpon Rouillé   ║\n");
    printf("║                                                           ║\n");
    printf("║                      PLONGEUR VS REQUIN                   ║\n");
    printf("║                       ♠        🦈                         ║\n");
    printf("║ 	             ════════▶ ◀════════                    ║\n");
    printf("║ Dégâts infligés: 22 points                                ║\n");
    printf("║ Le Requin-Tigre riposte! Vous perdez 18 points de vie     ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
}

void afficher_interface_combat();