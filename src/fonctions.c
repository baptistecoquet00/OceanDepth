#include "../include/fonctions.h"
#include <string.h>

// Fonctions mathématiques
int addition(int a, int b) {
    return a + b;
}

int soustraction(int a, int b) {
    return a - b;
}

int multiplication(int a, int b) {
    return a * b;
}

float division(int a, int b) {
    if (b == 0) {
        return 0.0f; // Évite la division par zéro
    }
    return (float)a / b;
}

// Fonctions de manipulation de chaînes
int longueur_chaine(const char *chaine) {
    int longueur = 0;
    while (chaine[longueur] != '\0') {
        longueur++;
    }
    return longueur;
}

void inverser_chaine(char *chaine) {
    int debut = 0;
    int fin = longueur_chaine(chaine) - 1;
    
    while (debut < fin) {
        char temp = chaine[debut];
        chaine[debut] = chaine[fin];
        chaine[fin] = temp;
        debut++;
        fin--;
    }
}

// Fonctions de combat
int calculer_degats(int attaque, int defense) {
    int degats = attaque - (defense / 2);
    return degats > 0 ? degats : 1; // Au moins 1 point de dégât
}

int est_en_vie(int points_de_vie) {
    return points_de_vie > 0;
}