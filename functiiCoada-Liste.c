/*VÎRLAN Mihnea-Alexandru - 311CB*/
/*-- functiiCoada-Liste.c -- elementele cozii sunt memorate într-o listă --*/
#include "TCoada-Liste.h"
#include "TCoada.h"
#include "TStiva-liste.h"

/*Funcție care inițializează o coadă*/
void* InitQ(size_t d)
{
	AQ a = (AQ)malloc(sizeof(TCoada));
  	if (!a)
		return NULL;
    a->dime=d;//atribuiesc dimensiunea maximă a unui element
    a->ic=NULL;//setez începutul și sfârșitul cu NULL
    a->sc=NULL;
  	return (void*)a;
}

/* adaugă element la sfârșitul cozii
    Cazul particular coadă (inițial) vidă*/
int InsCoadavida(void *c, void *ae)
{
    TLG aux;
    aux=(TLG)malloc(sizeof(TCelula));//aloc o celulă
    if(aux==NULL)
        return 0;
    aux->info=ae;//setez informația
    IC(c)=aux;
    SC(c)=aux;
    /*Am legat începutul și sfârșitul cozii la aceeași celulă*/
  	return 1;
}

/*Inserare pe cazul cozii nevide (avem cel puțin un element*/
int InsCoadanevida(void *c, void *ae)
{
    TLG aux;
    aux=(TLG)malloc(sizeof(TCelula));
    if(aux==NULL)
        return 0;
    aux->info=ae;
    SC(c)->urm=aux;
    SC(c)=aux;
    /*Se actualizează poziția cozii*/
    return 1;
}

/*Funcție care extrage un element listă simplu înlănțuită dintr-o coadă*/
int ExtragereCoada(void *c, void *ae)
{
    if((IC(c)==NULL)&&(SC(c)==NULL))
        return 0;
    TLG aux;
    aux=IC(c);
    /*actualizez începutul cozii*/
    IC(c)=aux->urm;
    memcpy(ae,aux->info,DIMEc(c));
    if(IC(c)==NULL)
        SC(c)=NULL;//dacă coada a avut un element sfârșitul se duce la NULL
    return 1;

}

void ResetQ(void *c)
{
	TLG aux,p;
	aux=IC(c);
	while(aux!=NULL)
	{
        p=aux;
        aux=aux->urm;
        free(p);
	}
	IC(c)=NULL;
	SC(c)=NULL;
}

/*Aici aveți o funcție care restează o coadă, având ca parametru suplimentar o funcție de ștergere element*/
void ResetQ_informatie_complexa(void *c, TF eliminare)
{
    TLG aux,p;
	aux=IC(c);
	while(aux!=NULL)
	{
        p=aux;
        aux=aux->urm;
        eliminare(p->info);
        free(p);
	}
	IC(c)=NULL;
	SC(c)=NULL;
}

void DistrQ(void **c)
{
    ResetQ(*c);
    free(*c);
    *c=NULL;
}

void DistrQ_complexa(void **c, TF eliminare)
{
    ResetQ_informatie_complexa(*c,eliminare);
    free(*c);
    *c=NULL;
}

void AfisareCoada(void *c,afisare_element AFISARE, FILE *fp)
{
    TLG aux;
    aux=IC(c);
    fprintf(fp,"[");
    while(aux!=NULL)
        {
            AFISARE(aux->info,fp);
            aux=aux->urm;
            if(aux!=NULL)
                fprintf(fp,", ");
        }
    fprintf(fp,"].\n");
}

int numara_elemente(void *c)
{
    TLG p;
    p=IC(c);
    int count=1;
    if(p==NULL)
        return 0;
    if(IC(c)==SC(c))
        return 1;
    while(p!=SC(c))
    {
    count++;
    p=p->urm;
    }
    return count;

}
