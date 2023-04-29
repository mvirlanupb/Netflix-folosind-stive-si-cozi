/*VÎRLAN Mihnea-Alexandru - 311CB*/
/*--- functiiLG.c -- operatii de baza pentru lista simplu inlantuita generica---*/
#include "tlg.h"
#define MAXIM_TOP10 10

int InsLG(TLG* aL, void* ae)//inserare inainte
{
	TLG aux = malloc(sizeof(TCelula));//alocăm memorie pentru o celulă tip listă
	if(!aux)
	    return 0;

	aux->info = ae;//atașăm informația
	aux->urm = *aL;//legăm celula nouă de începutul listei
	*aL = aux;//actualizăm începutul listei

	return 1;
}

int InserareDupa(TLG L, void *ae)
{
    TLG aux=malloc(sizeof(TCelula));//alocăm memorie pentru o celulă tip listă
    if(!aux)
        return 0;
    aux->info=ae;//atașăm informația
    aux->urm=L->urm;//legăm celula
    L->urm=aux;//actualizăm lista

    return 1;
}

void Distruge(TLG* aL, TF free_elem) /* distruge lista */
{
	while(*aL != NULL)
    {
        TLG aux = *aL;     /* adresa celulei eliminate */
        if (!aux)
            return;

        free_elem(aux->info);  /* elib.spatiul ocupat de element*/
        *aL = aux->urm;    /* deconecteaza celula din lista */
        free(aux);   /* elibereaza spatiul ocupat de celula */
    }
}

size_t LungimeLG(TLG* a)      /* numarul de elemente din lista */
{
	size_t lg = 0;
	TLG p = *a;

     /* parcurge lista, numarand celulele */
	for (; p != NULL; p = p->urm)
        lg++;

	return lg;
}

void Afisare(TLG* aL, TF_afi afiEL, FILE *fp)
{
	if(!*aL) {
        fprintf(fp,"[].\n");
        return;
    }

	fprintf(fp,"[");
	for(; *aL; aL = &(*aL)->urm) {
		afiEL((*aL)->info,fp);
		if((*aL)->urm!=NULL)
            fprintf(fp,", ");
    }
	fprintf(fp,"].\n");
}

/*Precum am menționat în sursa Seriale.c, inserarea la primele 3 categorii de seriale se face folosind
2 funcții de comparare. Prima funcție, f1 evaluează ordinea ratingurilor, iar a doua evaluează
superioritatea lexicografică. Se consideră că lista e de la început ordonată.*/
int inserare_doua_criterii(TLG *L, void *x, TFCmp f1, TFCmp f2)
{
    TLG p,ant;
    p=*L;
    /*Dacă am primit inițial o listă vidă, se face o inserare înainte cu elementul x primit ca parametru.*/
    if(*L==NULL)
        return InsLG(L,x);
    /*f1 întoarce 1 dacă ratingul primului argument (serial) e mai mare decât ratingul celui de-al doilea argument
    (serial). Dacă ratingul primit e mai mare decât ratingul serialului din prima poziție, se face o inserare
    înainte (la început de listă).*/
    if(f1(x,p->info)==1)
        return InsLG(L,x);
    /*f1 întoarce 0 dacă ratingurile sunt egale. Dacă ratingul primit coincide cu ratingul serialului din prima celulă, dar numele
    primit e inferior, se face inserare la început de listă*/
    if((f1(x,p->info)==0)&&(f2(x,p->info)==0))
        return InsLG(L,x);
    /*Parcurgem lista, astfel încât să ajungem la primul serial pentru care are ratingul este cel puțin egal cu ratingul serialului x (cel
    primit ca parametru. Folosim și un element TLG ant (anterior) pentru a păstra celula precedentă a lui p.*/
    while(p!=NULL)
    {
        if(f1(x,p->info)!=-1)
            break;
        ant=p;
        p=p->urm;
    }
    /*În cazul în care ratingul serialului primit e mai mic decât ratingul ultimului serial, se face inserarea după ultima celulă.
    p==NULL semnifică faptul că am parcurs toată lista, deci ratingurile tuturor serialelor sunt strict mai mari deccât ratingul
    lui x.*/
    if(p==NULL)
        return InserareDupa(ant,x);
    /*Dacă p s-a oprit în interiorul listei (fie și la ultima celulă) și ratingul lui x e mai mare decât ratingul lui p, se face
    inserarea după ant.
    Exemplu concret: vrem să introducem un serial cu rating 7.3 în următoarea situație
    8.1 8 7.9 7.7 7.6 7.4 7
                      ant p
    p s-a oprit la 7 pentru că 7.3>7. Cum 7.4>7.3 se face inserarea după ant (adică 7.4)*/
    if(f1(x,p->info)==1)
        return InserareDupa(ant,x);
    /*Acum suntem în situația în care ratingul lui x coincide cu cel al lui p. Parcurgem toate serialele ce au același rating cu cel
    al lui x, până când ajungem la un serial pentru care numele lui x este inferior lexicografic decât numele lui p
    Exemplu
    7    7    7   7   7   7    7       și vrem să introducem serialul 7
    AVG AYR  BVF CDE CGB DGR  EEA                                    CAD
              ant p
    p se va opri la CDE pentru ca CAD<CDE din punct de vedere lexicografic
    În acest caz, se va face inserarea după BVF (ant).*/
    while((f1(x,p->info)==0)&&(p->urm!=NULL))
    {
        if(f2(x,p->info)==0)//daca se ajunge la prima celulă cu nume inferior, ne oprim
            break;
        ant=p;
        p=p->urm;
    }
    /*Dacă totuși am ajuns la un rating mai mic se face inserare după ant.
    Exemplu
    7    7    7    6.8
    ABV BVV  CSA   VVV și inserăm serialul cu ratingul 7 și numele DDE
    atunci, cum p va fi la serialul VVV și rating 6.8, se va face inserarea după serialul CSA, reținut în ant. */
    if(f1(x,p->info)!=0)
        return InserareDupa(ant,x);
    if(f2(x,p->info)==0)
        return InserareDupa(ant,x);
    /*Pentru situația în care numele dat x e superior numelui lui p, se va face inserarea după p. Condiția aici este ca
    ratingul lui x să fie egal cu cel al lui p.
    Exemplu: 7   7    7    7   6.5
            CSQ DSW  EFR  GHT  SRE   și introducem serialul cu rating 7 și cu numele HUY
                      ant  p
    Cum numele lui x (HUY) e superior lui GHT se va face inserarea după p*/
    return InserareDupa(p,x);
}

int inserare_lista_finita(TLG *L, void *x, TFElem f, TF schimba, TF elimina)
{
    //f întoarce poziția din top 10
    //dacă poziția primita este 1, se insereaza înainte
    /*schimbă este o funcție care mărește cu 1 poziția în top pentru serialele aflate dedesubtul celui inserat
    elimină este o funcție care scoate din listă serialul care ar fi luat poziția 11 în clasament*/
    TLG p,ant,ant1;
    int rez;
    if(f(x)==1)
    {
        rez=InsLG(L,x);
        if(rez==0)
            return rez;
        if((*L)->urm==NULL)//situația în care lista are doar un singur element
            return 1;
        //daca avem la dreapta un serial marcat tot cu 1 (pozitie in top) îl ștergem
        for(p=(*L)->urm;p->urm!=NULL;ant=p,p=p->urm)
            schimba(p->info);//actualizăm toate ratingurile următoarelor seriale
        if(LungimeLG(L)<=MAXIM_TOP10)
            schimba(p->info);
        /*Dacă am depășit lungimea maximă admisibilă a listei, dăm afară ultima celulă fără a se pierde informația*/
        if(LungimeLG(L)>MAXIM_TOP10)
            {
            ant->urm=NULL;
            //elimina(p->info);
            free(p);
            }
        return 1;
    }
    /*Cazul în care serialul dat concurează pe o poziție diferită de 1 (ex. locul 2, 3...10)*/
    p=*L;
    size_t i;
    for(i=0;i<f(x)-1;i++)//ajungem la poziția pe care vrem să o ocupăm
        {
        ant=p;
        p=p->urm;
        }
    /*Dacă un serial concurează pentru poziția a zecea, ant care a reținut serialul de pe locul 9 va avea actualizată legătura,
    se șterge celula p (care ocupă locul 10) în locul lui p va veni x.*/
    if(f(x)==10)
    {
        ant->urm=NULL;
        free(p);
        rez=InserareDupa(ant,x);
        return 1;
    }
    /*În caz normal se face inserare după ant (care ocupă locul poziție-1)*/
    rez=InserareDupa(ant,x);
    if(rez==0)
        return rez;
    ant=ant->urm->urm;//mergem la celula imediat următoare a lui p
    if(ant==NULL)
        return 1;
    /*Dacă ant nu este ultima celulă, parcurgem lista și actualizăm pozițiile serialelor situate sub p.*/
    while(ant->urm!=NULL)
    {
        ant1=ant;
        schimba(ant->info);
        ant=ant->urm;
    }
    size_t lungime_finala_lista=LungimeLG(L);
    /*Dacă lungimea este cel mult 10 se actualizează și ultima celulă*/
    if(lungime_finala_lista<=MAXIM_TOP10)
        schimba(ant->info);
    /*Altfel o dăm afară*/
    if(lungime_finala_lista>MAXIM_TOP10)
    {
    ant1->urm=NULL;
    //elimina(ant->info);
    free(ant);
    }
    return 1;
}

/*Această funcție ignoră prima celulă a listei, fără a o elimina*/
void evita_primul_element(TLG *L)
{
    TLG p;
    p=*L;
    if(p==NULL)
        return;
    *L=p->urm;
}

/*Această funcție nu face decât să scape de un element din interiorul unei liste, aflat pe o poziție
dată fără a șterge informația.*/

void evita_element_interior(TLG L, int pozitie)
{
    TLG p,ant;
    p=L;
    int i;
    for(i=1;i<=pozitie-1;i++)
        {
            ant=p;
            p=p->urm;
        }
    ant->urm=p->urm;

}

void evita_ultimul_element(TLG L)
{
    TLG p,ant;
    for(p=L;p->urm!=NULL;ant=p, p=p->urm);
    ant->urm=NULL;
}

