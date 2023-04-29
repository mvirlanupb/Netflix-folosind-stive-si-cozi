/*VÎRLAN Mihnea-Alexandru - 311CB*/
/*-- TStiva.h --- tipul stiva ---*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "tlg.h"

#ifndef _FUNCTII_
#define _FUNCTII_
  typedef int (* TF1)(void *);
  typedef int (* TF2)(const void *, const void *);
  typedef void (*afisare)(void*,FILE*);
#endif

#ifndef STIVA
#define STIVA

void* InitS(size_t d);    /* initializeaza stiva cu elem.de dimensiune d;
				  					anumite implementari pot necesita si alti parametri */
int Push(void* s, void* ae);  /* pune element in varful stivei */
int Pop (void* s, void* ae);  /* extrage elementul din varful stivei la adresa ae */
int Top (void* s, void* ae);  /* copiaza elementul din varful stivei la adresa ae */
void ResetS (void* s, TF elimina);        /* transforma stiva in stiva vida */
void DistrS (void** s, TF elimina);      /* elibereaza intregul spatiu ocupat de stiva */
void Afiseaza_Stiva(void *s, TF_afi afiseaza, FILE *fp);
void muta_varful(void *s1, void *s2);
int aduce_in_varf(void *s, void *x, TFCmp f);

#endif
