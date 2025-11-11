#ifndef UTILITAIRES_SYSTEME_H
#define UTILITAIRES_SYSTEME_H

#include <stdio.h>

void effacer_ecran(void) {
    // Codes ANSI standards - fonctionnent sur la plupart des terminaux
    printf("\033[2J\033[1;1H");
    // \033[2J = effacer tout l'écran
    // \033[1;1H = positionner en haut à gauche
    fflush(stdout);
}

#endif