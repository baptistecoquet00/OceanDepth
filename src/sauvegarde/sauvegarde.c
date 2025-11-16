#include "../../include/sauvegarde.h"

int sauvegarder_plongeur(const char* nom_fichier, Plongeur* plongeur) {
    FILE* fichier = fopen(nom_fichier, "w");
    if (fichier == NULL) {
        printf("Erreur: Impossible de créer le fichier %s\n", nom_fichier);
        return -1;
    }

    fprintf(fichier, "{\n");
    fprintf(fichier, "    \"oceandepths_save_v1\": {\n");
    fprintf(fichier, "        \"plongeur\":{\n");
    fprintf(fichier, "            \"pv_plongeur\": %d,\n", plongeur->points_de_vie);
    fprintf(fichier, "            \"pv_max_plongeur\": %d,\n", plongeur->points_de_vie_max);
    fprintf(fichier, "            \"nv_oxygene\": %d,\n", plongeur->niveau_oxygene);
    fprintf(fichier, "            \"nv_max_oxygne\": %d,\n", plongeur->niveau_oxygene_max);
    fprintf(fichier, "            \"niveau_fatigue\": %d,\n", plongeur->niveau_fatigue);
    fprintf(fichier, "            \"perles\": %d\n", plongeur->perles);
    fprintf(fichier, "        },\n");

    fclose(fichier);
    return 0;
}

int sauvegarder_creatures(const char* nom_fichier, CreatureMarine* creatures, int nb_creatures) {
    FILE* fichier = fopen(nom_fichier, "a"); // Mode append pour ajouter aux données existantes
    if (fichier == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier %s\n", nom_fichier);
        return -1;
    }

    fprintf(fichier, "        \"creatures\": {\n");
    fprintf(fichier, "            \"nb_monstre\": %d,\n", nb_creatures);
    fprintf(fichier, "            \"state_creature\": [\n");
    
    for (int i = 0; i < nb_creatures; i++) {
        fprintf(fichier, "                {\n");
        fprintf(fichier, "                    \"nom_creature\": \"%s\",\n", creatures[i].nom);
        fprintf(fichier, "                    \"pv_creature\": %d,\n", creatures[i].points_de_vie_actuels);
        fprintf(fichier, "                    \"pv_max_creature\": %d,\n", creatures[i].points_de_vie_max);
        fprintf(fichier, "                    \"attaque_minimale\": %d,\n", creatures[i].attaque_minimale);
        fprintf(fichier, "                    \"attaque_maximale\": %d,\n", creatures[i].attaque_maximale);
        fprintf(fichier, "                    \"defense\": %d,\n", creatures[i].defense);
        fprintf(fichier, "                    \"vitesse\": %d,\n", creatures[i].vitesse);
        fprintf(fichier, "                    \"effet_special\": \"%s\"\n", creatures[i].effet_special);
        
        if (i < nb_creatures - 1) {
            fprintf(fichier, "                },\n");
        } else {
            fprintf(fichier, "                }\n");
        }
    }
    
    fprintf(fichier, "            ]\n");
    fprintf(fichier, "        }\n");
    fprintf(fichier, "    }\n");
    fprintf(fichier, "}\n");

    fclose(fichier);
    return 0;
}

int sauvegarder_jeu_complet(const char* nom_fichier, Plongeur* plongeur, CreatureMarine* creatures, int nb_creatures) {
    // Commencer par sauvegarder le plongeur (crée le fichier)
    if (sauvegarder_plongeur(nom_fichier, plongeur) != 0) {
        return -1;
    }
    
    // Ajouter les équipements (fixes pour l'instant)
    FILE* fichier = fopen(nom_fichier, "a");
    if (fichier == NULL) {
        return -1;
    }
    
    fprintf(fichier, "        \"equipement\": [\"Harpon_Rouille\",\"Neoprene_Basic\"],\n");
    
    // Ajouter la position (exemple fixe)
    fprintf(fichier, "        \"position\": {\n");
    fprintf(fichier, "            \"nv_zone\": -247,\n");
    fprintf(fichier, "            \"nom_zone\": \"Recif_Corallien\"\n");
    fprintf(fichier, "        },\n");
    
    fclose(fichier);
    
    // Enfin sauvegarder les créatures
    if (sauvegarder_creatures(nom_fichier, creatures, nb_creatures) != 0) {
        return -1;
    }
    
    printf("✅ Sauvegarde créée: %s\n", nom_fichier);
    return 0;
}

