#include "../include/sauvegarde.h"

// Fonctions d'extraction (restent les mêmes)
int extraire_valeur_numerique(const char *json, const char *cle) {
    char pattern[100];
    snprintf(pattern, sizeof(pattern), "\"%s\":", cle);
    
    const char *pos = strstr(json, pattern);
    if (!pos) return -1;
    
    pos += strlen(pattern);
    while (*pos && !isdigit(*pos) && *pos != '-') pos++;
    
    return atoi(pos);
}

void extraire_valeur_chaine(const char *json, const char *cle, char *buffer, size_t taille) {
    char pattern[100];
    snprintf(pattern, sizeof(pattern), "\"%s\":\"", cle);
    
    const char *pos = strstr(json, pattern);
    if (!pos) {
        buffer[0] = '\0';
        return;
    }
    
    pos += strlen(pattern);
    const char *fin = strchr(pos, '"');
    if (!fin) {
        buffer[0] = '\0';
        return;
    }
    
    size_t len = fin - pos;
    if (len >= taille) len = taille - 1;
    
    strncpy(buffer, pos, len);
    buffer[len] = '\0';
}

int compter_creatures(const char *json) {
    const char *pos = json;
    int count = 0;
    
    while ((pos = strstr(pos, "{\"nom_creature\"")) != NULL) {
        count++;
        pos++;
    }
    
    return count;
}

int charger_creatures(const char* nom_fichier, CreatureMarine** creatures, int* nb_creatures) {
    FILE* fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier %s\n", nom_fichier);
        return -1;
    }

    // Lire tout le contenu du fichier
    fseek(fichier, 0, SEEK_END);
    long taille = ftell(fichier);
    fseek(fichier, 0, SEEK_SET);
    
    char* contenu = (char*)malloc(taille + 1);
    if (contenu == NULL) {
        fclose(fichier);
        return -1;
    }
    
    fread(contenu, 1, taille, fichier);
    contenu[taille] = '\0';
    fclose(fichier);

    // Chercher le nombre de monstres
    char* nb_monstre_pos = strstr(contenu, "\"nb_monstre\"");
    if (nb_monstre_pos == NULL) {
        free(contenu);
        printf("Erreur: Champ nb_monstre non trouvé\n");
        return -1;
    }
    
    sscanf(nb_monstre_pos, "\"nb_monstre\": %d", nb_creatures);
    
    // Allouer le tableau de créatures
    *creatures = (CreatureMarine*)malloc(*nb_creatures * sizeof(CreatureMarine));
    if (*creatures == NULL) {
        free(contenu);
        return -1;
    }

    // Chercher le début du tableau state_creature
    char* state_creature_pos = strstr(contenu, "\"state_creature\": [");
    if (state_creature_pos == NULL) {
        free(contenu);
        free(*creatures);
        printf("Erreur: Tableau state_creature non trouvé\n");
        return -1;
    }

    // Pour chaque créature dans le tableau
    char* current_pos = state_creature_pos;
    for (int i = 0; i < *nb_creatures; i++) {
        // Chercher le prochain objet créature
        char* creature_start = strstr(current_pos, "{");
        if (creature_start == NULL) break;
        
        char* creature_end = strstr(creature_start, "}");
        if (creature_end == NULL) break;
        
        // Extraire les données de cette créature
        char* nom_pos = strstr(creature_start, "\"nom_creature\"");
        char* pv_pos = strstr(creature_start, "\"pv_creature\"");
        char* pv_max_pos = strstr(creature_start, "\"pv_max_creature\"");
        char* att_min_pos = strstr(creature_start, "\"attaque_minimale\"");
        char* att_max_pos = strstr(creature_start, "\"attaque_maximale\"");
        char* defense_pos = strstr(creature_start, "\"defense\"");
        char* vitesse_pos = strstr(creature_start, "\"vitesse\"");
        char* effet_pos = strstr(creature_start, "\"effet_special\"");
        
        if (nom_pos && pv_pos && pv_max_pos && att_min_pos && att_max_pos && 
            defense_pos && vitesse_pos && effet_pos) {
            
            char nom_temp[32];
            int pv_actuels, pv_max, att_min, att_max, defense, vitesse;
            char effet_temp[20];
            
            sscanf(nom_pos, "\"nom_creature\": \"%31[^\"]\"", nom_temp);
            sscanf(pv_pos, "\"pv_creature\": %d", &pv_actuels);
            sscanf(pv_max_pos, "\"pv_max_creature\": %d", &pv_max);
            sscanf(att_min_pos, "\"attaque_minimale\": %d", &att_min);
            sscanf(att_max_pos, "\"attaque_maximale\": %d", &att_max);
            sscanf(defense_pos, "\"defense\": %d", &defense);
            sscanf(vitesse_pos, "\"vitesse\": %d", &vitesse);
            sscanf(effet_pos, "\"effet_special\": \"%19[^\"]\"", effet_temp);
            
            // Remplir la structure
            (*creatures)[i].id = i + 1;
            strcpy((*creatures)[i].nom, nom_temp);
            (*creatures)[i].points_de_vie_max = pv_max;
            (*creatures)[i].points_de_vie_actuels = pv_actuels;
            (*creatures)[i].attaque_minimale = att_min;
            (*creatures)[i].attaque_maximale = att_max;
            (*creatures)[i].defense = defense;
            (*creatures)[i].vitesse = vitesse;
            strcpy((*creatures)[i].effet_special, effet_temp);
            (*creatures)[i].est_vivant = (pv_actuels > 0) ? 1 : 0;
        }
        
        current_pos = creature_end + 1;
    }
    
    free(contenu);
    return 0;
}


// Fonction principale de chargement
SauvegardeJeu* charger_sauvegarde_complete(const char* nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        printf("Erreur: impossible d'ouvrir %s\n", nom_fichier);
        return NULL;
    }
    
    // Lire tout le fichier
    fseek(fichier, 0, SEEK_END);
    long fichier_taille = ftell(fichier);
    fseek(fichier, 0, SEEK_SET);
    
    char *json_content = malloc(fichier_taille + 1);
    fread(json_content, 1, fichier_taille, fichier);
    json_content[fichier_taille] = '\0';
    fclose(fichier);
    
    // Allouer la structure de sauvegarde
    SauvegardeJeu *sauvegarde = malloc(sizeof(SauvegardeJeu));
    
    // Allouer et initialiser le plongeur
    sauvegarde->plongeur = malloc(sizeof(Plongeur));
    sauvegarde->plongeur->points_de_vie = extraire_valeur_numerique(json_content, "pv_plongeur");
    sauvegarde->plongeur->points_de_vie_max = extraire_valeur_numerique(json_content, "pv_max_plongeur");
    sauvegarde->plongeur->niveau_oxygene = extraire_valeur_numerique(json_content, "nv_oxygene");
    sauvegarde->plongeur->niveau_oxygene_max = extraire_valeur_numerique(json_content, "nv_max_oxygne");
    sauvegarde->plongeur->niveau_fatigue = extraire_valeur_numerique(json_content, "niveau_fatigue");
    sauvegarde->plongeur->perles = extraire_valeur_numerique(json_content, "perles");
    
    // Allouer et initialiser le combat_plongeur
    sauvegarde->combat_plongeur = malloc(sizeof(Combat_plongeur));
    sauvegarde->combat_plongeur->gestion_fatigue_vie = sauvegarde->plongeur; // Même structure
    
    // Extraire la position
    sauvegarde->nv_zone = extraire_valeur_numerique(json_content, "nv_zone");
    extraire_valeur_chaine(json_content, "nom_zone", sauvegarde->nom_zone, sizeof(sauvegarde->nom_zone));
    
    if (charger_creatures(nom_fichier, &sauvegarde->creatures, &sauvegarde->nb_creatures) != 0) {
        printf("Erreur lors du chargement des créatures\n");
        liberer_sauvegarde(sauvegarde);
        return NULL;
    }
    
    free(json_content);
    return sauvegarde;
}

// Fonction pour libérer la sauvegarde
void liberer_sauvegarde(SauvegardeJeu *sauvegarde) {
    if (!sauvegarde) return;
    
    // Libération dans l'ordre logique
    if (sauvegarde->plongeur) {
        free_plongeur(sauvegarde->plongeur);  // ✅ Libère le Plongeur
    }
    
    if (sauvegarde->combat_plongeur) {
        free_combat_plongeur(sauvegarde->combat_plongeur);  // ✅ Libère le Combat_plongeur
    }
    
    if (sauvegarde->creatures) {
        free(sauvegarde->creatures);  // ✅ Libère le tableau de créatures
    }
    
    free(sauvegarde);  // ✅ Libère la structure principale
}