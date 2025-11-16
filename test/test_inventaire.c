#include <stdio.h>
#include "../include/inventaire.h"

/* Version minimale de Joueur juste pour tester.
   Quand tu intègreras vraiment, tu utiliseras ta vraie struct Joueur. */
typedef struct Joueur
{
    int pv;
    int oxygene;
    int fatigue;
    int est_paralyse;
    Inventaire inventaire;
} Joueur;

int main(void)
{
    Joueur j;

    /* Init joueur + inventaire */
    j.pv = 50;
    j.oxygene = 60;
    j.fatigue = 0;
    j.est_paralyse = 0;
    init_inventaire(&j.inventaire);

    /* On ajoute quelques objets à la main pour tester l'affichage */
    ajouter_objet(&j.inventaire, OBJET_HARPON_ROUILLE, 1);
    ajouter_objet(&j.inventaire, OBJET_COMBI_NEOPRENE, 1);
    ajouter_objet(&j.inventaire, OBJET_CAPSULE_OXYGENE, 3);
    ajouter_objet(&j.inventaire, OBJET_TROUSSE_SOIN, 2);
    ajouter_objet(&j.inventaire, OBJET_STIMULANT_MARIN, 1);
    ajouter_objet(&j.inventaire, OBJET_ANTIDOTE, 1);

    /* On teste aussi une récompense générée aléatoirement */
    int qte;
    ObjetID obj = generer_objet_aleatoire(0, &qte);
    ajouter_objet(&j.inventaire, obj, qte);
    j.inventaire.total_perles += generer_perles_aleatoires(0);

    /* On affiche l’écran “TRÉSOR DÉCOUVERT” pour l’objet aléatoire */
    afficher_tresor(obj, qte);

    /* Puis on ouvre l’interface d’inventaire */
    interface_inventaire(&j.inventaire, (struct Joueur *)&j);

    printf("\nFin du test inventaire.\n");
    return 0;
}