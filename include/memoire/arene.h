#ifndef ARENE_H
#define ARENE_H

#include <stddef.h>


#if __STDC_VERSION__ >= 201112L
    #include <stdalign.h>
    #define ARENE_ALIGNEMENT(type) alignof(type)
#else
    #define ARENE_ALIGNEMENT(type) offsetof(struct { char* c; type d;}, d)
#endif

#ifndef ARENE_DEFAUT_ALIGNEMENT
    #define ARENE_DEFAUT_ALIGNEMENT ARENE_ALIGNEMENT(size_t)
#endif

typedef struct 
{
    char* region;
    size_t index;
    size_t taille_total;
} Arene;

Arene* nouvelle_arene(size_t taille);
Arene* etendre_arene(Arene* arene,size_t taille);
void* arene_allocation(Arene* arene,size_t taille);
void* arene_allocation_alignee(Arene* arene,size_t taille, unsigned int alignement);
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

Arene* nouvelle_arene(size_t taille){
    Arene* arene;

    if(taille == 0) 
    {
        return NULL;
    }

    arene = (Arene*)ARENE_MALLOC(sizeof(Arene));
    if(arene == NULL)
    {
        return NULL;
    }

    arene->region = ARENE_MALLOC(taille);
    if(arene->region == NULL)
    {
        ARENE_LIBRE(arene);
        return NULL;
    }

    arene->index = 0;
    arene->taille_total = taille;

    return arene;
}

Arene* etendre_arene(Arene* arene,size_t taille){
    if(arene == NULL || taille <= arene->taille_total)
    {
        return NULL;
    }

    arene->region = ARENE_REALLOC(arene->region, taille);

    if(arene->region == NULL)
    {
        return NULL;
    }

    arene->taille_total = taille;
    return arene;
}

void* arene_allocation(Arene* arene, size_t taille){
    return arene_allocation_alignee(arene,taille,ARENE_DEFAUT_ALIGNEMENT);
}

void* arene_allocation_alignee(Arene* arene, size_t taille,unsigned int alignement){
    unsigned int rembourrage = 0;

    if(arene == NULL || arene->region == NULL || taille == 0)
    {
        return NULL;
    }

    if( alignement != 0)
    {
        size_t compenser = (size_t)(arene->region + arene->index) % alignement;
        if(compenser > 0)
        {
            rembourrage =  alignement - compenser;
        } 
    }

    if(arene->index + rembourrage + taille > arene->taille_total)
    {
        return NULL;
    }

    void* ptr = arene->region + arene->index + rembourrage;
    arene->index += rembourrage + taille;
    return ptr;
}

void arene_nettoyer(Arene* arene){
    if(arene == NULL)
    {
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