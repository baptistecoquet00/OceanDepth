#include "inventaire.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Pour éviter de reseed rand() partout */
static void init_random_once(void)
{
    static int deja_fait = 0;

    if (!deja_fait)
    {
        srand((unsigned int)time(NULL));
        deja_fait = 1;
    }
}



static const DonneesObjet g_donnees_objets[NB_MODELES_OBJETS] =
{
    /* Harpons */

    [OBJET_HARPON_ROUILLE] = {
        OBJET_HARPON_ROUILLE,
        "Harpon Rouille",
        TYPE_OBJET_HARPON,
        QUALITE_COMMUNE,
        /* atk_min, atk_max, cout_oxygene_par_attaque */
        12, 18, 2,
        /* defense, cout_oxygene_par_tour */
        0, 0,
        /* bonus_pv, bonus_oxygene, bonus_fatigue, annule_paralysie */
        0, 0, 0, 0
    },

    [OBJET_HARPON_LASER] = {
        OBJET_HARPON_LASER,
        "Harpon Laser",
        TYPE_OBJET_HARPON,
        QUALITE_RARE,
        35, 50, 3,
        0, 0,
        0, 0, 0, 0
    },

    [OBJET_HARPON_ELECTRIQUE] = {
        OBJET_HARPON_ELECTRIQUE,
        "Harpon Electrique",
        TYPE_OBJET_HARPON,
        QUALITE_RARE,
        28, 42, 4,
        0, 0,
        0, 0, 0, 0
    },

    /* Combinaisons */

    [OBJET_COMBI_NEOPRENE] = {
        OBJET_COMBI_NEOPRENE,
        "Neoprene Basic",
        TYPE_OBJET_COMBI,
        QUALITE_COMMUNE,
        0, 0, 0,
        5, 1,
        0, 0, 0, 0
    },

    [OBJET_COMBI_TITANE] = {
        OBJET_COMBI_TITANE,
        "Armure Titanium",
        TYPE_OBJET_COMBI,
        QUALITE_RARE,
        0, 0, 0,
        25, 2,
        0, 0, 0, 0
    },

    /* Consommables */

    [OBJET_CAPSULE_OXYGENE] = {
        OBJET_CAPSULE_OXYGENE,
        "Capsule d'Oxygene",
        TYPE_OBJET_CONSO,
        QUALITE_COMMUNE,
        0, 0, 0,
        0, 0,
        0, 40, 0, 0
    },

    [OBJET_TROUSSE_SOIN] = {
        OBJET_TROUSSE_SOIN,
        "Trousse de Soin",
        TYPE_OBJET_CONSO,
        QUALITE_COMMUNE,
        0, 0, 0,
        0, 0,
        25, 0, 0, 0
    },

    [OBJET_STIMULANT_MARIN] = {
        OBJET_STIMULANT_MARIN,
        "Stimulant Marin",
        TYPE_OBJET_CONSO,
        QUALITE_COMMUNE,
        0, 0, 0,
        0, 0,
        0, 0, -2, 0
    },

    [OBJET_ANTIDOTE] = {
        OBJET_ANTIDOTE,
        "Antidote",
        TYPE_OBJET_CONSO,
        QUALITE_COMMUNE,
        0, 0, 0,
        0, 0,
        0, 0, 0, 1
    }
};

const DonneesObjet *get_donnees_objet(ObjetID id)
{
    if (id <= OBJET_AUCUN || id >= NB_MODELES_OBJETS)
        return NULL;

    return &g_donnees_objets[id];
}



void init_inventaire(Inventaire *inv)
{
    int i;

    if (!inv)
        return;

    for (i = 0; i < TAILLE_INVENTAIRE; ++i)
    {
        inv->slots[i].id = OBJET_AUCUN;
        inv->slots[i].quantite = 0;
    }

    inv->slot_harpon_equipe = -1;
    inv->slot_combi_equipe  = -1;
    inv->total_perles       = 0;
}

bool inventaire_plein(const Inventaire *inv)
{
    int i;

    if (!inv)
        return true;

    for (i = 0; i < TAILLE_INVENTAIRE; ++i)
    {
        if (inv->slots[i].id == OBJET_AUCUN)
            return false;
    }

    return true;
}

/* On empile uniquement les consommables */
static bool objet_empilable(ObjetID id)
{
    const DonneesObjet *d = get_donnees_objet(id);

    if (!d)
        return false;

    return (d->type == TYPE_OBJET_CONSO);
}

int ajouter_objet(Inventaire *inv, ObjetID id, int quantite)
{
    int i;

    if (!inv || quantite <= 0)
        return -1;

    /* Si empilable, on essaie d'abord de le stacker */
    if (objet_empilable(id))
    {
        for (i = 0; i < TAILLE_INVENTAIRE; ++i)
        {
            if (inv->slots[i].id == id)
            {
                inv->slots[i].quantite += quantite;
                return i;
            }
        }
    }

    /* Sinon, on cherche un slot vide */
    for (i = 0; i < TAILLE_INVENTAIRE; ++i)
    {
        if (inv->slots[i].id == OBJET_AUCUN)
        {
            inv->slots[i].id = id;
            inv->slots[i].quantite = quantite;
            return i;
        }
    }

    /* Plus de place */
    return -1;
}

void retirer_objet(Inventaire *inv, int index, int quantite)
{
    if (!inv)
        return;
    if (index < 0 || index >= TAILLE_INVENTAIRE)
        return;
    if (quantite <= 0)
        return;

    if (inv->slots[index].id == OBJET_AUCUN)
        return;

    inv->slots[index].quantite -= quantite;

    if (inv->slots[index].quantite <= 0)
    {
        inv->slots[index].id = OBJET_AUCUN;
        inv->slots[index].quantite = 0;

        if (inv->slot_harpon_equipe == index)
            inv->slot_harpon_equipe = -1;
        if (inv->slot_combi_equipe == index)
            inv->slot_combi_equipe = -1;
    }
}

void vider_inventaire(Inventaire *inv)
{
    init_inventaire(inv);
}



bool equiper_harpon(Inventaire *inv, int index)
{
    const DonneesObjet *d;

    if (!inv)
        return false;
    if (index < 0 || index >= TAILLE_INVENTAIRE)
        return false;

    d = get_donnees_objet(inv->slots[index].id);
    if (!d || d->type != TYPE_OBJET_HARPON)
        return false;

    inv->slot_harpon_equipe = index;
    return true;
}

bool equiper_combi(Inventaire *inv, int index)
{
    const DonneesObjet *d;

    if (!inv)
        return false;
    if (index < 0 || index >= TAILLE_INVENTAIRE)
        return false;

    d = get_donnees_objet(inv->slots[index].id);
    if (!d || d->type != TYPE_OBJET_COMBI)
        return false;

    inv->slot_combi_equipe = index;
    return true;
}



bool utiliser_objet(Inventaire *inv, int index, struct Joueur *joueur)
{
    const DonneesObjet *d;

    (void)joueur; /* à connecter plus tard avec ta vraie struct Joueur */

    if (!inv)
        return false;
    if (index < 0 || index >= TAILLE_INVENTAIRE)
        return false;
    if (inv->slots[index].id == OBJET_AUCUN)
        return false;

    d = get_donnees_objet(inv->slots[index].id);
    if (!d || d->type != TYPE_OBJET_CONSO)
        return false;

    printf("[INFO] Utilisation de %s :\n", d->nom);

    if (d->bonus_pv)
        printf("  -> TODO: +%d PV\n", d->bonus_pv);
    if (d->bonus_oxygene)
        printf("  -> TODO: +%d O2\n", d->bonus_oxygene);
    if (d->bonus_fatigue)
        printf("  -> TODO: fatigue %+d\n", d->bonus_fatigue);
    if (d->annule_paralysie)
        printf("  -> TODO: annuler paralysie/poison\n");

    retirer_objet(inv, index, 1);
    return true;
}



static const char *nom_qualite(QualiteObjet q)
{
    switch (q)
    {
        case QUALITE_COMMUNE: return "QUALITE COMMUNE";
        case QUALITE_RARE:    return "QUALITE RARE";
        default:              return "QUALITE ?";
    }
}

void afficher_tresor(ObjetID id, int quantite)
{
    const DonneesObjet *d = get_donnees_objet(id);

    if (!d)
        return;

    printf("============================================================\n");
    printf("||                      TRESOR DECOUVERT !               ||\n");
    printf("============================================================\n");
    printf("  HARPO N / OBJET : %s\n", d->nom);
    printf("  [%s]\n", nom_qualite(d->qualite));
    printf("  Quantite : %d\n", quantite);

    if (d->type == TYPE_OBJET_HARPON)
    {
        printf("  -> Attaque : %d-%d\n", d->atk_min, d->atk_max);
        printf("  -> Consommation : %d oxygene/attaque\n",
               d->cout_oxygene_par_attaque);
    }
    else if (d->type == TYPE_OBJET_COMBI)
    {
        printf("  -> Defense : +%d\n", d->defense);
        printf("  -> O2 : -%d/tour\n", d->cout_oxygene_par_tour);
    }
    else if (d->type == TYPE_OBJET_CONSO)
    {
        if (d->bonus_pv)
            printf("  -> Soin : +%d PV\n", d->bonus_pv);
        if (d->bonus_oxygene)
            printf("  -> O2 : +%d\n", d->bonus_oxygene);
        if (d->bonus_fatigue)
            printf("  -> Fatigue %+d\n", d->bonus_fatigue);
        if (d->annule_paralysie)
            printf("  -> Annule paralysie/poison\n");
    }

    printf("\n[Appuyez sur Entree pour continuer]\n");
    getchar();
}

static const char *nom_slot(const Inventaire *inv, int i)
{
    const DonneesObjet *d;

    if (!inv)
        return "(Erreur)";
    if (inv->slots[i].id == OBJET_AUCUN)
        return "(Vide)";

    d = get_donnees_objet(inv->slots[i].id);
    return d ? d->nom : "(Inconnu)";
}

void afficher_inventaire(const Inventaire *inv)
{
    int i;
    const DonneesObjet *harpon = NULL;
    const DonneesObjet *combi  = NULL;

    if (!inv)
        return;

    if (inv->slot_harpon_equipe >= 0 &&
        inv->slot_harpon_equipe < TAILLE_INVENTAIRE)
    {
        harpon = get_donnees_objet(inv->slots[inv->slot_harpon_equipe].id);
    }

    if (inv->slot_combi_equipe >= 0 &&
        inv->slot_combi_equipe < TAILLE_INVENTAIRE)
    {
        combi = get_donnees_objet(inv->slots[inv->slot_combi_equipe].id);
    }

    printf("============================================================\n");
    printf("  EQUIPE: HARPON: %-15s | EQUIPE: COMBI: %-15s | PERLES: %3d\n",
           harpon ? harpon->nom : "Aucun",
           combi  ? combi->nom  : "Aucune",
           inv->total_perles);
    printf("============================================================\n");

    for (i = 0; i < TAILLE_INVENTAIRE; ++i)
    {
        printf("[%d] %-20s", i + 1, nom_slot(inv, i));

        if (inv->slots[i].id != OBJET_AUCUN)
            printf(" x%d", inv->slots[i].quantite);

        if (i == inv->slot_harpon_equipe)
            printf("  (Harpon equipe)");
        if (i == inv->slot_combi_equipe)
            printf("  (Combi equipe)");

        printf("\n");
    }

    printf("------------------------------------------------------------\n");
    printf("1-Utiliser objet  2-Equiper harpon  3-Equiper combi  4-Retour\n");
    printf("Choix : ");
}

static int lire_entier(void)
{
    char buffer[64];

    if (!fgets(buffer, sizeof buffer, stdin))
        return 0;

    return atoi(buffer);
}

void interface_inventaire(Inventaire *inv, struct Joueur *joueur)
{
    int quitter = 0;

    while (!quitter)
    {
        int choix;

        afficher_inventaire(inv);
        choix = lire_entier();

        switch (choix)
        {
            case 1:
            {
                int index;
                printf("Numero de slot a utiliser (1-%d) : ", TAILLE_INVENTAIRE);
                index = lire_entier() - 1;
                if (!utiliser_objet(inv, index, joueur))
                    printf("Impossible d'utiliser cet objet.\n");
                break;
            }

            case 2:
            {
                int index;
                printf("Slot du harpon a equiper (1-%d) : ", TAILLE_INVENTAIRE);
                index = lire_entier() - 1;
                if (!equiper_harpon(inv, index))
                    printf("Ce slot ne contient pas de harpon.\n");
                break;
            }

            case 3:
            {
                int index;
                printf("Slot de la combinaison a equiper (1-%d) : ", TAILLE_INVENTAIRE);
                index = lire_entier() - 1;
                if (!equiper_combi(inv, index))
                    printf("Ce slot ne contient pas de combinaison.\n");
                break;
            }

            case 4:
                quitter = 1;
                break;

            default:
                printf("Choix invalide.\n");
                break;
        }

        if (!quitter)
        {
            printf("\n[Appuyez sur Entree pour continuer]\n");
            getchar();
        }
    }
}


ObjetID generer_objet_aleatoire(int creature_puissante, int *quantite_out)
{
    int r;

    init_random_once();

    /* pour l’instant on donne toujours 1 exemplaire */
    if (quantite_out)
        *quantite_out = 1;

    r = rand() % 100;

    if (!creature_puissante)
    {
        if (r < 40) return OBJET_CAPSULE_OXYGENE;
        if (r < 65) return OBJET_TROUSSE_SOIN;
        if (r < 80) return OBJET_STIMULANT_MARIN;
        if (r < 90) return OBJET_HARPON_ROUILLE;
        return OBJET_COMBI_NEOPRENE;
    }
    else
    {
        if (r < 20) return OBJET_CAPSULE_OXYGENE;
        if (r < 35) return OBJET_TROUSSE_SOIN;
        if (r < 50) return OBJET_STIMULANT_MARIN;
        if (r < 65) return OBJET_ANTIDOTE;
        if (r < 80) return OBJET_HARPON_LASER;
        if (r < 90) return OBJET_COMBI_TITANE;
        return OBJET_HARPON_ELECTRIQUE;
    }
}

/* Génère un nombre de perles selon le type de créature */
int generer_perles_aleatoires(int creature_puissante)
{
    int min, max;

    init_random_once();

    if (!creature_puissante)
    {
        min = 5;
        max = 15;
    }
    else
    {
        min = 20;
        max = 50;
    }

    return min + (rand() % (max - min + 1));
}