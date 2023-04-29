/*VÎRLAN Mihnea-Alexandru - 311CB*/
/*-- TCoada.h --- tipul coada ---*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "tlg.h"

#ifndef _FUNCTII_
#define _FUNCTII_
  typedef int (*TF1)(void *);
  typedef int (*TF2)(const void *, const void *);
  typedef void (*afisare_element)(void*,FILE*);
#endif

#ifndef COADA
#define COADA

void* InitQ(size_t d); /* creeaza coada vida cu elemente de dimensiune d;*/
int InsCoadavida(void *c, void *ae);
int InsCoadanevida(void *c, void *ae);
int ExtragereCoada(void *c, void *ae);
void ResetQ(void* c);         /* transforma coada in coada vida */
void DistrQ(void** c);       /* elibereaza intregul spatiu ocupat de coada */
void ResetQ_informatie_complexa(void *c, TF eliminare);
void DistrQ_complexa(void **c, TF eliminare);
void AfisareCoada(void *c,afisare_element AFISARE, FILE *fp);
int numara_elemente(void *c);
int din_coada_in_stiva(void *c, void *s, void *informatie, TFCmp f, TFCmp f1, size_t d);
#endif
