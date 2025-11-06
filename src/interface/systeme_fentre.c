#include "../../include/systeme_fenetre.h"

void nouvelle_gf(GestionFenetre *gf,Arene *arene){
    
}
Fenetre* gf_creer_fenetre(GestionFenetre *gf, int x,int y,int l,int h,const char *titre){}
void gf_detruire_fenetre(GestionFenetre *gf,Fenetre *fenetre){}
void gf_rendu(GestionFenetre *gf){}

void fenetre_nettoyer(Fenetre *fenetre){}
void fenetre_dessiner_bordure(Fenetre *fenetre){}
void afficher_fenetre(Fenetre *fenetre, int x,int y, const char *texte){}
void afficher_fenetre(Fenetre *fenetre,int x, int y , const char *format,...){}
