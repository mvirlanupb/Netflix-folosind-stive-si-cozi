/*VÎRLAN Mihnea-Alexandru - 311CB*/
#include "TCoada-Liste.h"
#ifndef _SERIALE_
#define _SERIALE_

typedef struct
{
    int ID;
    char *nume_serial;
    float rating;
    int nr_sezoane;
    int pozitie_top;
    int durata_vizionare;
    void *coada_sezoanelor;
}Serial;

typedef struct
{
    int nr_episoade;
    void *coada_episoadelor;
}Sezon;

void afisare_serial(void *x, FILE *fp);
void stergere_sezon(void *a);
void stergere_serial(void *a);
int citire_serial(TLG *L, TFCmp f1, TFCmp f2, FILE *fp);
int adauga_sezon_serial_lista(TLG L, char *nume_referinta, FILE *fp);
int citire_serial_top10(TLG *L10, TFElem f, TF schimba, TF elimina, FILE *fp);
void scoate_din_lista(TLG *L, char *nume_referinta);
void scoate_din_top10(TLG *L, char *nume_referinta, TF schimba);
int adaugare_in_stiva_watching_din_lista(void *s, void *s1, char *nume_serial_dat, int timp, TLG *L, FILE *fp);
int actualizeaza_in_stiva(void *s, void *s1, char *nume_serial_dat, int timp, TFCmp f, FILE *fp);
int adauga_serial_in_coada(void *c, char *nume_referinta, TLG *L, FILE *fp);
int adauga_din_top10_in_coada(void *c, char *nume_referinta, TLG *L10, TF schimba, FILE *fp);
int adauga_in_stiva_din_coada(void *s, void *s1, void *c, char *nume_referinta, int timp, TFCmp f, FILE *fp);
int adauga_in_stiva_din_top10(void *s, void *s1, TLG *L10, char *nume_referinta, int timp, TF schimba, FILE *fp);
int adauga_sezon_serial_coada(void *c, char *nume_referinta, FILE *fp);
int adauga_sezon_serial_stiva(void *s, char *nume_referinta, FILE *fp);

#endif
