#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "../include/utilitaire.h"
#include "../include/joueur.h"
#include "../include/competences.h"
#include "../include/creatures.h"

void test_initialiser_competences() {
    Plongeur *joueur = nouveau_plongeur();
    GestionnaireCompetences *gc = initialiser_competences(joueur);
    
    printf("Test initialisation competences... ");
    assert(gc != NULL);
    assert(gc->nb_competences == 4);
    assert(gc->joueur_ref == joueur);
    
    // Verifier que chaque competence est bien initialisee
    for (int i = 0; i < 4; i++) {
        assert(gc->competences[i].id > 0);
        assert(strlen(gc->competences[i].nom) > 0);
        assert(gc->competences[i].cooldown_actuel == 0);
    }
    
    liberer_competences(gc);
    free_plongeur(joueur);
    ok();
}

void test_competence_utilisable() {
    Plongeur *joueur = nouveau_plongeur();
    GestionnaireCompetences *gc = initialiser_competences(joueur);
    
    printf("Test verification utilisation competences... ");
    
    // Apnee prolongee devrait etre utilisable (cout 0 oxygene)
    assert(competence_utilisable(gc, APNEE_PROLONGEE) == true);
    
    // Decharge electrique devrait etre utilisable (cout 18, O2=100)
    assert(competence_utilisable(gc, DECHARGE_ELECTRIQUE) == true);
    
    // Si oxygene < 18, decharge ne doit pas etre utilisable
    joueur->niveau_oxygene = 10;
    assert(competence_utilisable(gc, DECHARGE_ELECTRIQUE) == false);
    
    liberer_competences(gc);
    free_plongeur(joueur);
    ok();
}

void test_utiliser_apnee_prolongee() {
    Plongeur *joueur = nouveau_plongeur();
    GestionnaireCompetences *gc = initialiser_competences(joueur);
    
    printf("Test utilisation Apnee Prolongee... ");
    
    joueur->niveau_oxygene = 50;  // Reduire pour tester l'ajout
    
    CreatureMarine *cibles[] = {NULL};
    int resultat = utiliser_competence(gc, APNEE_PROLONGEE, cibles, 0);
    
    assert(resultat == 0);
    assert(joueur->niveau_oxygene == 70);  // 50 + 20
    assert(gc->competences[0].cooldown_actuel == 4);  // Cooldown active
    
    liberer_competences(gc);
    free_plongeur(joueur);
    ok();
}

void test_utiliser_decharge_electrique() {
    Plongeur *joueur = nouveau_plongeur();
    GestionnaireCompetences *gc = initialiser_competences(joueur);
    
    printf("Test utilisation Decharge Electrique... ");
    srand(time(NULL));
    
    // Creer 2 creatures test
    CreatureMarine creature1, creature2;
    creature1.id = 1;
    strcpy(creature1.nom, "Requin");
    creature1.points_de_vie_actuels = 50;
    creature1.est_vivant = 1;
    
    creature2.id = 2;
    strcpy(creature2.nom, "Meduse");
    creature2.points_de_vie_actuels = 40;
    creature2.est_vivant = 1;
    
    CreatureMarine *cibles[] = {&creature1, &creature2};
    
    int oxygene_avant = joueur->niveau_oxygene;
    int pv_creature1_avant = creature1.points_de_vie_actuels;
    int pv_creature2_avant = creature2.points_de_vie_actuels;
    
    int resultat = utiliser_competence(gc, DECHARGE_ELECTRIQUE, cibles, 2);
    
    assert(resultat == 0);
    assert(joueur->niveau_oxygene == oxygene_avant - 18);
    assert(creature1.points_de_vie_actuels < pv_creature1_avant);
    assert(creature2.points_de_vie_actuels < pv_creature2_avant);
    
    liberer_competences(gc);
    free_plongeur(joueur);
    ok();
}

void test_cooldown_reduction() {
    Plongeur *joueur = nouveau_plongeur();
    GestionnaireCompetences *gc = initialiser_competences(joueur);
    
    printf("Test reduction cooldown... ");
    
    // Utiliser Apnee (cooldown 4)
    CreatureMarine *cibles[] = {NULL};
    utiliser_competence(gc, APNEE_PROLONGEE, cibles, 0);
    assert(gc->competences[0].cooldown_actuel == 4);
    
    // Reduire les cooldowns
    reduire_cooldowns(gc);
    assert(gc->competences[0].cooldown_actuel == 3);
    
    reduire_cooldowns(gc);
    assert(gc->competences[0].cooldown_actuel == 2);
    
    reduire_cooldowns(gc);
    assert(gc->competences[0].cooldown_actuel == 1);
    
    reduire_cooldowns(gc);
    assert(gc->competences[0].cooldown_actuel == 0);
    
    liberer_competences(gc);
    free_plongeur(joueur);
    ok();
}

void test_recuperation_oxygene() {
    Plongeur *joueur = nouveau_plongeur();
    
    printf("Test recuperation oxygene naturelle... ");
    
    joueur->niveau_oxygene = 50;
    recuperation_oxygene_naturelle(joueur, true);  // En combat: +8
    assert(joueur->niveau_oxygene == 58);
    
    recuperation_oxygene_naturelle(joueur, false); // En exploration: +15
    assert(joueur->niveau_oxygene == 73);
    
    joueur->niveau_oxygene = 95;  // Changer de 90 à 95 pour eviter depasser 100
    recuperation_oxygene_naturelle(joueur, true);  // +8 = 103, cape a 100
    assert(joueur->niveau_oxygene == 100);  // Max atteint
    
    free_plongeur(joueur);
    ok();
}

void test_oxygene_critique() {
    Plongeur *joueur = nouveau_plongeur();
    
    printf("Test detection oxygene critique... ");
    
    joueur->niveau_oxygene = 50;
    assert(oxygene_est_critique(joueur) == false);
    
    joueur->niveau_oxygene = 11;
    assert(oxygene_est_critique(joueur) == false);
    
    joueur->niveau_oxygene = 9;
    assert(oxygene_est_critique(joueur) == true);
    
    joueur->niveau_oxygene = 0;
    assert(oxygene_est_critique(joueur) == true);
    
    free_plongeur(joueur);
    ok();
}

void test_capsule_oxygene() {
    Plongeur *joueur = nouveau_plongeur();
    
    printf("Test utilisation capsule oxygene... ");
    
    joueur->niveau_oxygene = 50;
    utiliser_capsule_oxygene(joueur);
    assert(joueur->niveau_oxygene == 90);  // 50 + 40
    
    utiliser_capsule_oxygene(joueur);
    assert(joueur->niveau_oxygene == 100);  // Max atteint
    
    // Impossible d'utiliser si deja au max
    int resultat = utiliser_capsule_oxygene(joueur);
    assert(resultat == -1);
    
    free_plongeur(joueur);
    ok();
}

int main() {
    printf("\n");
    printf("====================================\n");
    printf("   TESTS DES COMPETENCES AQUATIQUES\n");
    printf("====================================\n");
    printf("\n");
    
    debut_test();
    test_initialiser_competences();
    test_competence_utilisable();
    test_utiliser_apnee_prolongee();
    test_utiliser_decharge_electrique();
    test_cooldown_reduction();
    test_recuperation_oxygene();
    test_oxygene_critique();
    test_capsule_oxygene();
    tous_test_passe();
    
    return 0;
}
