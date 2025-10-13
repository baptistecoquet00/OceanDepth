#ifndef ARENE_H
#define ARENE_H

#include <stddef.h>



#if __STDC_VERSION__ >= 201112L
    #include <stdalign.h>
    #define ARENE_ALIGNEMENT (type) alignof(type)
#else
    #define ARENE_ALIGNEMENT (type) offsetof(struct { char* c; type d;}, d)
#endif

#ifndef ARENE_DEFAUT_ALIGNEMENT
    #define ARENE_DEFAUT_ALIGNEMENT ARENE_ALIGNEMENT(int)
#endif

typedef struct 
{
    char* region;
    int index;
    int taille_total;
} Arene;

Arene* nouvelle_arene(int taille);
Arene* etendre_arene(Arene* arene,int taille);
void* arene_allocation(Arene* arene,int taille);
void* arene_allocation_alignee(Arene* arene, int taille, int alignement);
void arene_nettoyer(Arene* arene);
void arene_detruite(Arene* arene);

#ifdef ARENE_IMPLEMENTATION

#ifndef ARENE_MALLOC
    #include <stdlib.h>
    #define ARENE_MALLOC malloc
#endif

#ifndef ARENE_LIBRE
    #include <stdlib.h>
    #define ARENE_LIBRE free
#endif

#ifndef ARENE_REALLOC
    #include <stdlib.h>
    #define ARENE_REALLOC realloc
#endif

#ifndef ARENE_COPIEMROIRE
    #include <string.h>
    #define ARENE_COPIEMROIRE memcpy
#endif

Arene* nouvelle_arene(int taille){
    Arene* arene;

    if(taille == 0) {
        return NULL;
    }

    arene = (Arene*)ARENE_MALLOC(sizeof(Arene));
    if(arene == NULL){
        return NULL;
    }

    arene->region = ARENE_MALLOC(taille);
    if(arene->region == NULL){
        ARENE_LIBRE(arene);
        return NULL;
    }

    arene->index = 0;
    arene->taille_total = taille;

    return arene;
}

Arene* etendre_arene(Arene* arene,int taille){
    if(arene == NULL || taille <= arene->taile){
        return NULL;
    }

    arene->region = ARENE_REALLOC(arene->region, taille);

    if(arene->region == NULL){
        return NULL;
    }

    arene->taille_total = taille;
    return arena;
}

void* arene_allocation(Arene* arene,int taille){
    return arene_allocation_alignee(arene,taille,ARENE_DEFAUT_ALIGNEMENT);
}

void* arene_allocation_alignee(Arene* arene, int taille,unsigned int alignement){
    unsigned int compenser;

    if(taille == 0){
        return NULL;
    }
    
    if(arene == NULL || arene->region == NULL){
        return NULL;
    }

    if( alignement != 0){
        compenser = (arene->region + taille) % alignement;
        if(compenser > 0){
            arene->index = arene->region - compenser + alignement;
        } 
    }else {
        compenser = 0;
    }

    if(arene->taille_total - (arene->index + compenser) < taille){
        return NULL;
    }
    arene->taille_total += taille;
    return arene->region + (arene->index - taille);
}
void arene_nettoyer(Arene* arene){
    if(arene == NULL){
        return;
    }
    arene->index = 0;
}
void arene_detruite(Arene* arene){
    if(arene == NULL){
        return;
    }
    if(arene->region != NULL){
        ARENE_LIBRE(arene->region);
    }

    ARENE_LIBRE(arene);
}

#endif /*ARENE_IMPLEMENTATION*/

#endif /* ARENE_H*/