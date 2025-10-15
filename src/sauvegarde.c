#include <stdio.h>
#include <string.h>
#include "sauvegarde.h"

int save_game(const char *filename, Plongeur *p, Inventaire *inv, int profondeur, CreatureMarine creatures[], int nb) {
    FILE *f = fopen(filename, "w");
    if (!f) return 0;
    fprintf(f, "OCEANDEPTHS_SAVE_V1\n");
    fprintf(f, "PLONGEUR|%d|%d|%d|%d|%d|%d|%s|%s\n",
            p->points_de_vie, p->points_de_vie_max, p->niveau_oxygene, p->niveau_oxygene_max,
            p->niveau_fatigue, p->perles, p->harpon, p->combinaison);
    fprintf(f, "INVENTAIRE|%d", inv->nb_objets);
    for (int i = 0; i < MAX_SLOTS; i++) {
        if (inv->slots[i].nom[0] != '\0') {
            fprintf(f, "|%s:%d", inv->slots[i].nom, inv->slots[i].quantite);
        }
    }
    fprintf(f, "\n");
    fprintf(f, "POSITION|%d\n", profondeur);
    fprintf(f, "CREATURES|%d", nb);
    for (int i = 0; i < nb; i++) {
        fprintf(f, "|%s:%d:%d:%d:%d:%d", creatures[i].nom, creatures[i].points_de_vie_actuels,
                creatures[i].points_de_vie_max, creatures[i].attaque_minimale,
                creatures[i].attaque_maximale, creatures[i].defense);
    }
    fprintf(f, "\n");
    fclose(f);
    return 1;
}

int load_game(const char *filename, Plongeur *p, Inventaire *inv, int *profondeur, CreatureMarine creatures[], int *nb) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "PLONGEUR|", 9) == 0) {
            int pv, pvmax, oxy, oxymax, fat, per;
            char harpon[64], combi[64];
            sscanf(line+9, "%d|%d|%d|%d|%d|%d|%63[^|]|%63[^\n]", &pv, &pvmax, &oxy, &oxymax, &fat, &per, harpon, combi);
            p->points_de_vie = pv; p->points_de_vie_max = pvmax;
            p->niveau_oxygene = oxy; p->niveau_oxygene_max = oxymax;
            p->niveau_fatigue = fat; p->perles = per;
            strncpy(p->harpon, harpon, sizeof(p->harpon)-1);
            strncpy(p->combinaison, combi, sizeof(p->combinaison)-1);
        } else if (strncmp(line, "INVENTAIRE|", 11) == 0) {
            init_inventaire(inv);
            char *pstr = line + 11;
            int nbobj = 0;
            sscanf(pstr, "%d", &nbobj);
            char *tok = strtok(pstr, "|");
            int count = 0;
            while (tok) {
                if (count > 0) {
                    char name[64]; int q;
                    if (sscanf(tok, "%63[^:]:%d", name, &q) == 2) {
                        ajouter_objet(inv, name, q);
                    }
                }
                tok = strtok(NULL, "|");
                count++;
            }
        } else if (strncmp(line, "POSITION|", 9) == 0) {
            sscanf(line+9, "%d", profondeur);
        } else if (strncmp(line, "CREATURES|", 10) == 0) {
            int n;
            char *pstr = line + 10;
            sscanf(pstr, "%d", &n);
            *nb = n;
            // parse creatures sequentially
            char *tok = strtok(pstr, "|");
            int idx = 0;
            while (tok && idx < *nb) {
                if (idx == 0) { tok = strtok(NULL, "|"); continue; } // skip first token which was count
                char name[64]; int hp, hpm, amin, amax, def;
                if (sscanf(tok, "%63[^:]:%d:%d:%d:%d:%d", name, &hp, &hpm, &amin, &amax, &def) == 6) {
                    strncpy(creatures[idx-1].nom, name, sizeof(creatures[idx-1].nom)-1);
                    creatures[idx-1].points_de_vie_actuels = hp;
                    creatures[idx-1].points_de_vie_max = hpm;
                    creatures[idx-1].attaque_minimale = amin;
                    creatures[idx-1].attaque_maximale = amax;
                    creatures[idx-1].defense = def;
                    creatures[idx-1].est_vivant = (hp > 0);
                }
                tok = strtok(NULL, "|");
                idx++;
            }
        }
    }
    fclose(f);
    return 1;
}
