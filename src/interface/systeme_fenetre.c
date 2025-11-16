
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "../../include/utilitaires_systeme.h" 
#define ARENE_IMPLEMENTATION
#include "../../include/gm.h"
#include "../../include/systeme_fenetre.h"

void nouvelle_gf(GestionFenetre *gf,Arene *arene){
    memset(gf,0,sizeof(GestionFenetre));
    gf->nb_fenetre = 0;
    gf->arene = arene;
}
Fenetre* gf_creer_fenetre(GestionFenetre *gf, int x,int y,int l,int h,const char *titre){
    if(gf->nb_fenetre >= MAX_FENETRE || gf->arene == NULL) return NULL;

    Fenetre *fenetre = &gf->fenetres[gf->nb_fenetre];
    fenetre->x = x;
    fenetre->y = y;
    fenetre->hauteur = h;
    fenetre->largeur = l;
    fenetre->visible = true;
    fenetre->z_index = gf->nb_fenetre;
    strncpy(fenetre->titre,titre,sizeof(fenetre->titre)-1);

    size_t tampon_taille = l * h * sizeof(char);
    fenetre->tampon = arene_allocation(gf->arene, tampon_taille);

    if(fenetre->tampon) memset(fenetre->tampon,' ', l *h);
    else return NULL ;

    gf->nb_fenetre++;
    return fenetre;
    
}
void gf_detruire_fenetre(GestionFenetre *gf,Fenetre *fenetre){
    if(gf == NULL || fenetre == NULL) return;
    fenetre->visible = false;
    fenetre->tampon = NULL;
}

void gf_rendu(GestionFenetre *gf){
    printf("\033[2J\033[1;1H");
    printf("=== GF_RENDU APPELE ===\n");
    fflush(stdout);
    // Rendu de toutes les fenêtres visibles
    for(int i = 0; i < gf->nb_fenetre; i++) {
        Fenetre *fenetre = &gf->fenetres[i];
        if(!fenetre->visible || !fenetre->tampon) continue;

        for(int y = 0; y < fenetre->hauteur; y++){
            printf("\033[%d;%dH",fenetre->y + y,fenetre->x);
            for (int x = 0; x < fenetre->largeur; x++) {
                putchar(fenetre->tampon[y * fenetre->largeur + x]);
            }
        }
    }
    printf("\n");
    fflush(stdout);
}

// void fenetre_nettoyer(Fenetre *fenetre){
//     if(fenetre->tampon){
//         memset(fenetre->tampon,' ',fenetre->largeur * fenetre->hauteur);
//     }
// }

void fenetre_nettoyer(Fenetre *fenetre){
    if(fenetre->tampon){
        memset(fenetre->tampon,' ',fenetre->largeur * fenetre->hauteur);
        fenetre_dessiner_bordure(fenetre);
    }
}

void fenetre_dessiner_bordure_debug(Fenetre *fen) {
    if (!fen || !fen->tampon) return;
    
    // Remplir tout avec '.'
    for (int y = 0; y < fen->hauteur; y++) {
        for (int x = 0; x < fen->largeur; x++) {
            fen->tampon[y * fen->largeur + x] = '.';
        }
    }
    
    // Bordures seulement
    for (int i = 0; i < fen->largeur; i++) {
        fen->tampon[i] = 'H'; // Haut
        fen->tampon[(fen->hauteur - 1) * fen->largeur + i] = 'B'; // Bas
    }
    
    for (int i = 0; i < fen->hauteur; i++) {
        fen->tampon[i * fen->largeur] = 'G'; // Gauche
        fen->tampon[i * fen->largeur + fen->largeur - 1] = 'D'; // Droite
    }
    
    // Coins
    fen->tampon[0] = '1';
    fen->tampon[fen->largeur - 1] = '2';
    fen->tampon[(fen->hauteur - 1) * fen->largeur] = '3';
    fen->tampon[(fen->hauteur - 1) * fen->largeur + fen->largeur - 1] = '4';
}

void fenetre_dessiner_bordure(Fenetre *fen) {
    if (!fen || !fen->tampon) return;
    
    // 1. Remplir TOUTE la fenêtre avec des espaces
    for (int y = 0; y < fen->hauteur; y++) {
        for (int x = 0; x < fen->largeur; x++) {
            fen->tampon[y * fen->largeur + x] = ' ';
        }
    }
    
    // 2. Dessiner les bordures SANS les écraser après
    for (int i = 0; i < fen->largeur; i++) {
        fen->tampon[i] = '-'; // Haut
        fen->tampon[(fen->hauteur - 1) * fen->largeur + i] = '-'; // Bas
    }
    
    for (int i = 0; i < fen->hauteur; i++) {
        fen->tampon[i * fen->largeur] = '|'; // Gauche
        fen->tampon[i * fen->largeur + fen->largeur - 1] = '|'; // Droite
    }
    
    // 3. Coins
    fen->tampon[0] = '+';
    fen->tampon[fen->largeur - 1] = '+';
    fen->tampon[(fen->hauteur - 1) * fen->largeur] = '+';
    fen->tampon[(fen->hauteur - 1) * fen->largeur + fen->largeur - 1] = '+';
    
    // 4. Titre (écrit sur la bordure du haut)
    if (strlen(fen->titre) > 0) {
        int pos_titre = (fen->largeur - strlen(fen->titre)) / 2;
        if (pos_titre < 1) pos_titre = 1;
        if (pos_titre + strlen(fen->titre) < fen->largeur - 1) {
            strncpy(&fen->tampon[pos_titre], fen->titre, strlen(fen->titre));
        }
    }
}

void afficher_fenetre(Fenetre *fenetre, int x,int y, const char *texte){
    if (!fenetre || !fenetre->tampon || !texte){
        //printf("DEBUG: Parametres invalides\n");
        return;
    } 
    
    // Ajuster les coordonnées pour l'intérieur de la fenêtre
    // x=1, y=1 est le premier caractère à l'intérieur (après la bordure)
    int x_interne = x + 1;  // +1 pour éviter la bordure gauche
    int y_interne = y + 1;  // +1 pour éviter la bordure haut
    
    //printf("DEBUG afficher_fenetre: x=%d->%d, y=%d->%d, texte='%s'\n", x, x_interne, y, y_interne, texte);

    if (x_interne < 1 || y_interne < 1 || x_interne >= fenetre->largeur - 1 || y_interne >= fenetre->hauteur - 1) {
        return; // En dehors de la zone interne
    }
    
    int pos = y_interne * fenetre->largeur + x_interne;
    int max_chars = fenetre->largeur - x_interne - 1; // -1 pour bordure droite
    
    //printf("DEBUG: pos=%d, max_chars=%d\n", pos, max_chars);

    int chars_en_copie = strlen(texte);
    if (chars_en_copie > max_chars) {
        chars_en_copie = max_chars;
    }
    
    memcpy(&fenetre->tampon[pos], texte, chars_en_copie);
    //printf("DEBUG: %d caracteres copies\n", chars_en_copie);
}

void gf_rendu_debug(GestionFenetre *gf){
    printf("\033[2J\033[1;1H");
    
    for(int i = 0; i < gf->nb_fenetre; i++) {
        Fenetre *fenetre = &gf->fenetres[i];
        if(!fenetre->visible || !fenetre->tampon) continue;

        printf("Rendu fenetre %d à (%d,%d) taille %dx%d\n", 
               i, fenetre->x, fenetre->y, fenetre->largeur, fenetre->hauteur);
        
        for(int y = 0; y < fenetre->hauteur; y++){
            printf("\033[%d;%dH", fenetre->y + y, fenetre->x);
            for (int x = 0; x < fenetre->largeur; x++) {
                char c = fenetre->tampon[y * fenetre->largeur + x];
                putchar(c);
            }
            // Affiche la position après chaque ligne
            printf("  ← ligne %d", y);
        }
    }
    
    fflush(stdout);
}

// void afficher_fenetref(Fenetre *fenetre,int x, int y , const char *format,...){
//     if(fenetre == NULL || fenetre->tampon == NULL){
//         printf("DEBUG: Fenetre ou tampon NULL\n");
//         return;
//     }

//     if(x < 0 || y > 0 || x >= fenetre->largeur || y >= fenetre->hauteur){
//         return;
//     }

//     // Tampon temporaire pour formater la chaîne
//     char tampon[256];
//     va_list args;

//     va_start(args, format);
//     vsnprintf(tampon, sizeof(tampon), format, args);
//     va_end(args);
//     printf("DEBUG afficher_fenetref: x=%d, y=%d, texte='%s'\n", x, y, tampon);
//     afficher_fenetre(fenetre,x,y,tampon);
// }

void afficher_fenetref(Fenetre *fen, int x, int y, const char *format, ...) {
    //printf("DEBUG afficher_fenetref DEBUT: x=%d, y=%d, format='%s'\n", x, y, format);
    
    if (!fen || !fen->tampon) {
        //printf("DEBUG: Fenetre ou tampon NULL\n");
        return;
    }
    
    char buffer[256];
    va_list args;
    va_start(args, format);
    int longueur = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    //printf("DEBUG: buffer='%s', longueur=%d\n", buffer, longueur);
    
    // Appel à afficher_fenetre
    afficher_fenetre(fen, x, y, buffer);
    
    //printf("DEBUG afficher_fenetref FIN\n");
}