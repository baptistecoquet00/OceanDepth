#ifndef INVENTAIRE_H
#define INVENTAIRE_H

#include <stdbool.h>

#define TAILLE_INVENTAIRE 8

typedef enum
{
    TYPE_OBJET_HARPON,
    TYPE_OBJET_COMBI,
    TYPE_OBJET_CONSO,
    TYPE_OBJET_MONNAIE
} TypeObjet;

typedef enum
{
    QUALITE_COMMUNE,
    QUALITE_RARE
} QualiteObjet;

typedef enum
{
    OBJET_AUCUN = -1,
    OBJET_HARPON_ROUILLE,
    OBJET_HARPON_LASER,
    OBJET_HARPON_ELECTRIQUE,
    OBJET_COMBI_NEOPRENE,
    OBJET_COMBI_TITANE,
    OBJET_CAPSULE_OXYGENE,
    OBJET_TROUSSE_SOIN,
    OBJET_STIMULANT_MARIN,
    OBJET_ANTIDOTE,
    NB_MODELES_OBJETS
} ObjetID;

typedef struct
{
    ObjetID id;
    const char *nom;
    TypeObjet type;
    QualiteObjet qualite;

    int atk_min;
    int atk_max;
    int cout_oxygene_par_attaque;

    int defense;
    int cout_oxygene_par_tour;

    int bonus_pv;
    int bonus_oxygene;
    int bonus_fatigue;
    int annule_paralysie;

} DonneesObjet;

typedef struct
{
    ObjetID id;
    int quantite;
} SlotInventaire;

typedef struct
{
    SlotInventaire slots[TAILLE_INVENTAIRE];
    int slot_harpon_equipe;
    int slot_combi_equipe;
    int total_perles;

} Inventaire;

struct Joueur;

const DonneesObjet *get_donnees_objet(ObjetID id);

void init_inventaire(Inventaire *inv);
bool inventaire_plein(const Inventaire *inv);
int ajouter_objet(Inventaire *inv, ObjetID id, int quantite);
void retirer_objet(Inventaire *inv, int index, int quantite);
void vider_inventaire(Inventaire *inv);

bool equiper_harpon(Inventaire *inv, int index);
bool equiper_combi(Inventaire *inv, int index);
bool utiliser_objet(Inventaire *inv, int index, struct Joueur *joueur);

void afficher_tresor(ObjetID id, int quantite);
void afficher_inventaire(const Inventaire *inv);
void interface_inventaire(Inventaire *inv, struct Joueur *joueur);

ObjetID generer_objet_aleatoire(int creature_puissante, int *quantite_out);
int generer_perles_aleatoires(int creature_puissante);

#endif