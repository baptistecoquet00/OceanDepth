#ifndef FONCTIONS_H
#define FONCTIONS_H

// Fonctions mathématiques de base
int addition(int a, int b);
int soustraction(int a, int b);
int multiplication(int a, int b);
float division(int a, int b);

// Fonctions de manipulation de chaînes
int longueur_chaine(const char *chaine);
void inverser_chaine(char *chaine);

// Fonctions de combat (exemple)
int calculer_degats(int attaque, int defense);
int est_en_vie(int points_de_vie);

#endif