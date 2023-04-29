/*VÎRLAN Mihnea-Alexandru - 311CB*/
/*-- tlg.h --- LISTA SIMPLU INLANTUITA GENERICA ---*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#ifndef _LISTA_GENERICA_
#define _LISTA_GENERICA_

typedef struct celula
{
  void* info;           /* adresa informatie */
  struct celula *urm;   /* adresa urmatoarei celule */
} TCelula, *TLG; /* tipurile Celula, Lista  */


typedef int (*TFElem)(void*);     /* functie prelucrare element */
typedef int (*TFCmp)(void*, void*); /* functie de comparare doua elemente */
typedef void (*TF)(void*);
typedef void (*TF_afi)(void*, FILE*);     /* functie afisare/eliberare un element */

/* functii lista generica */
int InsLG(TLG*, void*);  /*- inserare la inceput reusita sau nu (1/0) -*/
int InserareDupa(TLG L, void *ae);
void Distruge(TLG* aL, TF fe); /* distruge lista */
size_t LungimeLG(TLG*);   /* numarul de elemente din lista */
/* afiseaza elementele din lista, folosind o functie de tip TFAfi */
void Afisare(TLG*, TF_afi, FILE *fp);
int inserare_doua_criterii(TLG *L, void *x, TFCmp f1, TFCmp f2);
int inserare_lista_finita(TLG *L, void *x, TFElem f, TF schimba, TF elimina);
void evita_primul_element(TLG *L);
void evita_element_interior(TLG L, int pozitie);
void evita_ultimul_element(TLG L);
#endif
