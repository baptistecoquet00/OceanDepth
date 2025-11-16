#ifndef INTERFACE_CARTE_H
#define INTERFACE_CARTE_H

#include "carte.h"
#include "systeme_fenetre.h"

typedef struct {
    Fenetre *carte_win;
    Fenetre *info_win;
    Fenetre *commande_win;
    bool visible;
} InterfaceCarte;

// Initialisation
InterfaceCarte* creer_interface_carte(GestionFenetre *gf);

// Affichage
void afficher_carte(InterfaceCarte *interface, const CarteOcean *carte);
void afficher_infos_zone(InterfaceCarte *interface, const ZoneCarte *zone);
void afficher_commandes(InterfaceCarte *interface);

// Mise à jour
void mettre_a_jour_interface_carte(InterfaceCarte *interface, const CarteOcean *carte);

void cacher_interface_carte(InterfaceCarte *interface);
void montrer_interface_carte(InterfaceCarte *interface);

// Nettoyage
void detruire_interface_carte(InterfaceCarte *interface);

#endif