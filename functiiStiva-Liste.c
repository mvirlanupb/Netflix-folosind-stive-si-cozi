/*VÎRLAN Mihnea-Alexandru - 311CB*/
#include "TStiva-liste.h"
#include "tlg.h"
#include "TStiva.h"

void *InitS(size_t d)
{
	ASt s = (ASt)malloc(sizeof (TStiva));
	if(!s)
        return NULL;
    s->dime=d;
    s->vf=NULL;
  	return (void *)s;
}

/* pune element in varful stivei */
int Push(void *s, void *ae)
{
    TLG aux;
    aux=(TLG)malloc(sizeof(TCelula));
    if(!aux)
        return 0;
    aux->info=ae;
    aux->urm=VF(s);
    VF(s)=aux;
  	return 1;
}

/* extrage elementul din varful stivei la adresa ae */
int Pop(void *s, void *ae)
{
    TLG aux=VF(s);
    if(aux==NULL) return 0;
    memcpy(ae,aux->info,DIME(s));
    VF(s)=aux->urm;//repozitionez varful stivei
    free(aux);
  	return 1;
}

/* transforma stiva in stiva vida */
void ResetS(void *s, TF elimina)
{
	TLG aux=VF(s),p;
	while(aux!=NULL)
	{
        p=aux;
        aux=p->urm;
        elimina(p->info);
        free(p);
	}
	VF(s)=NULL;
}

/* elibereaza intregul spatiu ocupat de stiva */
void DistrS(void **s, TF elimina)
{
    ResetS(*s,elimina);
    free(*s);
    *s=NULL;
}

void Afiseaza_Stiva(void *s, TF_afi afiseaza, FILE *fp)
{
    TLG p;
    fprintf(fp,"[");
    for(p=VF(s);p!=NULL;)
        {
        afiseaza(p->info,fp);
        p=p->urm;
        if(p!=NULL)
            fprintf(fp,", ");
        }
    fprintf(fp,"].\n");

}

void muta_varful(void *s1, void *s2)
{
    void *ae=malloc(DIME(s1));//inițiez elementul
    Pop(s1,ae);//scot din prima stivă
    Push(s2,ae);//mut în a doua stivă
}

int aduce_in_varf(void *s, void *x, TFCmp f)
{
    TLG p;
    p=VF(s);
    if(f(p->info,x)==1)//elementul se află deja în vârf
        return 0;
    void *s1, *aux;
    s1=InitS(DIME(s));//inițiez o stivă auxiliară
    aux=malloc(DIME(s));
    /*Cât timp elementul căutat nu se află în vârful stivei, mut elementele de deasupra acestuia în stiva
    auxiliară.*/
    while(f(VF(s)->info,x)==0)
        muta_varful(s,s1);
    Pop(s,aux);//am adus elementul căutat în vârf, deci îl scot și îl stochez temporar într-o variabilă auxiliară
    while(VF(s1)!=NULL)//pun restul elementelor la loc
        muta_varful(s1,s);
    Push(s,aux);//aduc variabila auxiliară în vârful stivei
    return 1;
}
