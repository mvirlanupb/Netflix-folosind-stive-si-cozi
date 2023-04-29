/*VÎRLAN Mihnea-Alexandru - 311CB*/
#include "Seriale.h"
#include "tlg.h"
#include "TStiva-liste.h"

/*Funcție care afișează numele unui serial și ratingul său*/
void afisare_serial(void *x, FILE *fp)
{
   Serial *sl=(Serial*)x;
   fprintf(fp,"(%s, %.1f)",sl->nume_serial,sl->rating);
}
/*Funcție care șterge coada corespunzătoare unui sezon*/
void stergere_sezon(void *a)
{
    Sezon *sz=(Sezon *)a;
    DistrQ(&sz->coada_episoadelor);
}
/*Funcție care șterge un serial*/
void stergere_serial(void *a)
{
    Serial *sl=(Serial *)a;
    free(sl->nume_serial);
    DistrQ_complexa(&sl->coada_sezoanelor,stergere_sezon);
    free(a);
}
/*Pentru comanda add, am pregătit o funcție care primește ca parametru o linie de fișier în care
se află informațiile necesare unui serial din primele 3 categorii, două funcții de comparare cu care se
face inserarea în listă, lista în care dorim să facem inserarea și un File pointer pentru a afișa mesajul cerut*/
int citire_serial(TLG *L,TFCmp f1, TFCmp f2, FILE *fp)
{
    TLG p;
    Serial *sl=(Serial*)malloc(sizeof(Serial));//inițierea unei structuri de tip serial
    if(sl==NULL)
        return 0;
    /*Pentru că în linia de fișier am un șir de caractere, folosesc strtok pentru a extrage
    informații și pentru a le converti la un alt tip de date, așa cum este necesar.*/
    char *ID_string=strtok(NULL," ");//extrag ID-ul
    int ID_1=atoi(ID_string);//transform în număr întreg
    char *nume_serial_string=strtok(NULL," ");//extrag numele serialului
    char *rating_string=strtok(NULL," ");//extrag ratingul
    float rating_1=atof(rating_string);//convertesc ratingul la float
    char *nr_sezoane_string=strtok(NULL," ");//extrag numărul de sezoane
    int nr_sezoane_1=atoi(nr_sezoane_string);//îl transform ca număr întreg
    /*Aloc memorie pentru numele serialului*/
    sl->nume_serial=(char*)malloc((strlen(nume_serial_string)+1)*sizeof(char));
    sl->ID=ID_1;
    /*Pentru că nu cunosc pe moment durata de vizionare a serialului, o inițiez cu 0*/
    sl->durata_vizionare=0;
    strcpy(sl->nume_serial,nume_serial_string);
    sl->rating=rating_1;
    sl->nr_sezoane=nr_sezoane_1;
    /*Fiind vorba despre primele 3 categorii, am setat, prin convenție, poziția în top10 cu 0.*/
    sl->pozitie_top=0;
    sl->coada_sezoanelor=InitQ(sizeof(Sezon));//inițiez coada_sezoanelor astfel încât să primească doar elemente de tip sezon
    //trebuie sa initializez fiecare sezon in parte, apoi il adaug
    if(sl->coada_sezoanelor==NULL)
        {
            free(sl);
            return 0;
        }
    int i,j;
    /*Facem o parcurgere în linia de fișier pentru a adăuga structurii Serial, coada sezoanelor, iar pentru
    fiecare coadă a sezoanelor, o coadă a episoadelor care va stoca durata fiecărui episod, număr întreg.*/
    for(i=0;i<nr_sezoane_1;i++)
    {
        Sezon *sz=(Sezon *)malloc(sizeof(Sezon));
        /*Am pregătit structura unui sezon*/
        if(sz==NULL)
            {
            DistrQ_complexa(&sl->coada_sezoanelor,stergere_sezon);
            free(sl);
            return 0;
            }
        char *nr_episoade_string=strtok(NULL," ");
        /*Pentru fiecare sezon extrag numărul de episoade*/
        int nr_episoade_1=atoi(nr_episoade_string);
        sz->nr_episoade=nr_episoade_1;
        /*Inițiez coada episoadelor. Va stoca duratele fiecăui episod.*/
        sz->coada_episoadelor=InitQ(sizeof(int));
        if(sz==NULL)
        {
            free(sz);
            DistrQ_complexa(&sl->coada_sezoanelor,stergere_sezon);
            free(sl->nume_serial);
            free(sl);
            return 0;
        }
        /*Pentru că eu cunosc numărul de episoade pentru sezonul citit, pregătesc un vector*/
        int *v=(int*)malloc(nr_episoade_1*sizeof(int));
        if(v==NULL)
        {
        DistrQ(&sz->coada_episoadelor);
        free(sz);
        DistrQ_complexa(&sl->coada_sezoanelor,stergere_sezon);
        free(sl->nume_serial);
        free(sl);
        return 0;
        }
        /*Adaug duratele fiecărui episod în coada episoadelor*/
        for(j=1;j<=nr_episoade_1;j++)
            {
            char *durata_episod_string=strtok(NULL," ");//extrag durata
            int durata_episod=atoi(durata_episod_string);//o convertesc la număr întreg
            /*Creștem durata totală de vizionare și adăugăm fiecare durată de episod în coada episoadelor*/
            (sl->durata_vizionare)=(sl->durata_vizionare)+durata_episod;
            v[j-1]=durata_episod;
            if(j==1)
                InsCoadavida(sz->coada_episoadelor,&v[j-1]);
            else
                InsCoadanevida(sz->coada_episoadelor,&v[j-1]);
            }
        if(i==0)
            InsCoadavida(sl->coada_sezoanelor,sz);
        else
            InsCoadanevida(sl->coada_sezoanelor,sz);
    }
    int rez,pozitie=0;
    /*Pentru că pregătirea structurii de tip Serial a avut succes, o adaug în listă
    Inițiez un număr întreg denumit poziție pentru că dorim să vedem care este poziția
    serialului în listă.*/
    rez=inserare_doua_criterii(L,sl,f1,f2);
    if(rez==1)
    {
        for(p=*L;p!=NULL;p=p->urm)
        {
            pozitie++;
            Serial *sl1=(Serial*)p->info;
            if(strcmp(sl1->nume_serial,sl->nume_serial)==0)
                break;

        }
        fprintf(fp,"Serialul %s a fost adaugat la pozitia %d.\n",sl->nume_serial,pozitie);
    }
    return rez;
}
/*Funcție care primește ca parametru Lista unde se află serialul, numele serialului pe care vrem să-l căutăm în listă
și un file pointer și care adaugă un sezon. Citirea sezonului se face la fel ca la funcția de citire a unui serial.*/
int adauga_sezon_serial_lista(TLG L, char *nume_referinta, FILE *fp)
{
    TLG p;
    /*Mai întâi se caută serialul în listă. Dacă nu s-a găsit în listă, funcția întoarce 0.*/
    for(p=L;p!=NULL;p=p->urm)
    {
        Serial *sl=(Serial *)p->info;
        if(strcmp(sl->nume_serial,nume_referinta)==0)
            break;

    }
    if(p==NULL)
        return 0;
    Serial *sl1=(Serial *)p->info;
    (sl1->nr_sezoane)++;
    Sezon *sz=(Sezon *)malloc(sizeof(Sezon));
    if(sz==NULL)
        return 0;
    char *nr_episoade_string=strtok(NULL," ");
    int nr_episoade_1=atoi(nr_episoade_string);
    sz->nr_episoade=nr_episoade_1;
    sz->coada_episoadelor=InitQ(sizeof(int));
    int i;
    int *v=(int*)malloc(nr_episoade_1*sizeof(int));
    for(i=0;i<nr_episoade_1;i++)
        {
        char *durata_episod_string=strtok(NULL," ");
        v[i]=atoi(durata_episod_string);
        (sl1->durata_vizionare)=(sl1->durata_vizionare)+v[i];
        if(i==0)
            InsCoadavida(sz->coada_episoadelor,&v[i]);
        else
            InsCoadanevida(sz->coada_episoadelor,&v[i]);
        }
    InsCoadanevida(sl1->coada_sezoanelor,sz);
    fprintf(fp,"Serialul %s are un sezon nou.\n",nume_referinta);
    return 1;
}

/*Funcția de citire și adăugare a unui serial în lista top10 se face asemănător cu citirea unui serial
din celelalte 3 categorii, însă cu următoarele diferențe: f este funcția care returnează ratingul unui serial,
schimbă este o funcție care actualizează poziția unui serial în top10, acolo unde este cazul,
iar elimina este o funcție care scoate un serial din top10. În sursa destinată funcțiilor de prelucrare
a listelor simplu înlănțuite vor fi detalii suplimentare despre folosirea acestor funcții.*/
int citire_serial_top10(TLG *L10, TFElem f, TF schimba, TF elimina, FILE *fp)
{
    Serial *sl=(Serial *)malloc(sizeof(Serial));
    if(sl==NULL)
        return 0;
    char *indice_top_string=strtok(NULL," ");
    int indice_top_nr=atoi(indice_top_string);
    sl->pozitie_top=indice_top_nr;
    char *nume_serial_string=strtok(NULL," ");
    char *rating_string=strtok(NULL," ");
    float rating_1=atof(rating_string);
    char *nr_sezoane_string=strtok(NULL," ");
    int nr_sezoane_1=atoi(nr_sezoane_string);
    sl->nume_serial=(char*)malloc((strlen(nume_serial_string)+1)*sizeof(char));
    sl->ID=0;
    /*O altă diferență stă în atribuirea unui ID. Am decis să atribui ID-ul unui serial din top10
    cu 0.*/
    strcpy(sl->nume_serial,nume_serial_string);
    sl->rating=rating_1;
    sl->durata_vizionare=0;
    sl->nr_sezoane=nr_sezoane_1;
    sl->coada_sezoanelor=InitQ(sizeof(Sezon));//initiez coada_sezoanelor astfel incat sa primeasca doar elemente de tip sezon
    //trebuie sa initializez fiecare sezon in parte, apoi il adaug
    if(sl->coada_sezoanelor==NULL)
        {
            free(sl);
            return 0;
        }
    int i,j;
    for(i=0;i<nr_sezoane_1;i++)
    {
        Sezon *sz=(Sezon *)malloc(sizeof(Sezon));
        if(sz==NULL)
            {
            DistrQ_complexa(&sl->coada_sezoanelor,stergere_sezon);
            free(sl);
            return 0;
            }
        char *nr_episoade_string=strtok(NULL," ");
        int nr_episoade_1=atoi(nr_episoade_string);
        sz->nr_episoade=nr_episoade_1;
        sz->coada_episoadelor=InitQ(sizeof(int));
        if(sz==NULL)
        {
            free(sz);
            DistrQ_complexa(&sl->coada_sezoanelor,stergere_sezon);
            free(sl->nume_serial);
            free(sl);
            return 0;
        }
        int *v=(int*)malloc(nr_episoade_1*sizeof(int));
        if(v==NULL)
        {
        DistrQ(sz->coada_episoadelor);
        free(sz);
        DistrQ_complexa(&sl->coada_sezoanelor,stergere_sezon);
        free(sl);
        return 0;
        }
        for(j=1;j<=nr_episoade_1;j++)
            {
            char *durata_episod_string=strtok(NULL," ");
            int durata_episod=atoi(durata_episod_string);
            v[j-1]=durata_episod;
            (sl->durata_vizionare)=(sl->durata_vizionare)+durata_episod;
            if(j==1)
                InsCoadavida(sz->coada_episoadelor,&v[j-1]);
            else
                InsCoadanevida(sz->coada_episoadelor,&v[j-1]);
            }
        if(i==0)
            InsCoadavida(sl->coada_sezoanelor,sz);
        else
            InsCoadanevida(sl->coada_sezoanelor,sz);
    }
    int rez;

    rez=inserare_lista_finita(L10,sl,f,schimba,elimina);
    if(rez==1)
        {
        fprintf(fp,"Categoria top10: ");
        Afisare(L10,afisare_serial,fp);
        return 1;
        }
    return rez;

}

/*Observație: de-a lungul acestui fișier sursă vor fi prezentate funcții în care sunt folosite ca parametrii
numele serialului. O consider o alegere strict personală.*/

/*Funcție care primește numele unui serial din primele 3 categorii și o listă în care dorim să facem căutarea.
Dacă serialul a fost găsit, va fi scos. Sunt folosite funcții aplicate pe liste simplu înlănțuite generice,
puse într-un alt fișier sursă.*/
void scoate_din_lista(TLG *L, char *nume_referinta)
{
    TLG p;
    int poz_in_lista=0;
    for(p=*L;p!=NULL;p=p->urm)
    {
        poz_in_lista++;
        Serial *sl=(Serial *)p->info;
        if(strcmp(sl->nume_serial,nume_referinta)==0)
            break;

    }
    /*În funcție de poziția în listă, distingem următoarele situații:
    1. Serialul se află la începutul listei.
    2. Serialul se află în interiorul listei
    3. Serialul se află la finalul listei*/
    if(poz_in_lista==1)
        evita_primul_element(L);
    else
    {
        if(poz_in_lista<(int)LungimeLG(L))
            evita_element_interior(*L,poz_in_lista);
        else
            evita_ultimul_element(*L);
    }

}
/*Asemănător cu funcția de mai sus, însă apare și o funcție care schimbă poziția în clasament
pentru serialele aflate dedesubtul celui scos (dacă există)*/
void scoate_din_top10(TLG *L, char *nume_referinta, TF schimba)
{
    TLG p,q;
    for(p=*L;p!=NULL;p=p->urm)
    {
        Serial *sl=(Serial *)p->info;
        if(strcmp(sl->nume_serial,nume_referinta)==0)
            break;
    }
    Serial *sl1=(Serial *)p->info;
    int pozitie=sl1->pozitie_top;
    if(p==*L)
    {
        evita_primul_element(L);
        for(q=*L;q!=NULL;q=q->urm)
            schimba(q->info);
    }
    else
    {
        if(p->urm!=NULL)/*în cazul în care avem seriale după cel pe care vrem să-l scoatem
                        le schimbăm poziția. Funcția primită ca parametru decrementează pozițiile
                        serialelor aflate sub cel pe care vrem să-l scoatem.*/
        {
            for(q=p->urm;q!=NULL;q=q->urm)
                    schimba(q->info);
            evita_element_interior(*L,pozitie);
        }
        else
            evita_ultimul_element(*L);
    }

}

/*Funcția trasnferă un serial dintr-o listă în stiva currently watching. Stiva s este ținta principală,
mai exact currently_watching, iar stiva s1 este stiva în care vrem să punem un serial vizionat complet.*/

int adaugare_in_stiva_watching_din_lista(void *s, void *s1, char *nume_serial_dat, int timp, TLG *L, FILE *fp)
{
    TLG p;
    for(p=*L;p!=NULL;p=p->urm)
    {
        Serial *sl=(Serial *)p->info;
        if(strcmp(sl->nume_serial,nume_serial_dat)==0)
            break;

    }
    if(p==NULL)
        return 0;
    Serial *sl=(Serial *)p->info;
    /*Dacă durata (totală) de vizionare a unui serial este cel mult egală cu timpul dorit de vizionare,
    serialul este mutat în stiva history*/
    if(sl->durata_vizionare<=timp)
    {
        sl->durata_vizionare=0;
        fprintf(fp,"Serialul %s a fost vizionat integral.\n",nume_serial_dat);
        scoate_din_lista(L,nume_serial_dat);
        void *element=malloc(sizeof(Serial));
        memcpy(element,p->info,sizeof(Serial));
        Push(s1,element);
        return 1;
    }
    /*În caz contrar scădem din durata totală de vizionare și trimitem în stiva s.*/
    sl->durata_vizionare=sl->durata_vizionare-timp;
    scoate_din_lista(L,nume_serial_dat);
    void *element=malloc(sizeof(Serial));
    memcpy(element,sl,sizeof(Serial));
    Push(s,element);
    return 1;
}
/*Această trimite în vârful stivei s(currently_watching) serialul pe care vrem să-l vedem. Funcția de comparare este folosită în cadrul
unei funcții definită în fișierul sursă destinat prelucrării Stivelor.*/
int actualizeaza_in_stiva(void *s, void *s1, char *nume_serial_dat, int timp, TFCmp f, FILE *fp)
{
    TLG p;
    for(p=VF(s);p!=NULL;p=p->urm)
    {
        Serial *sl=(Serial *)p->info;
        if(strcmp(sl->nume_serial,nume_serial_dat)==0)
            break;
    }
    if(p==NULL)
        return 0;
    aduce_in_varf(s,p->info,f);
    void *element;
    /*Pointer care va păstra vârful stivei.*/
    element=malloc(sizeof(Serial));
    /*Prelucrăm informația*/
    Serial *sl1=(Serial *)p->info;
    if(sl1->durata_vizionare<=timp)
        {
        sl1->durata_vizionare=0;
        fprintf(fp,"Serialul %s a fost vizionat integral.\n",nume_serial_dat);
        Pop(s,element);
        Push(s1,element);
        }
    else
        sl1->durata_vizionare=sl1->durata_vizionare-timp;
    return 1;
}
/*Funcție care verifică prezența serialului cu nume dat ca parametru într-o listă, urmând a fi mutată într-o coadă, aici
currently watching.*/
int adauga_serial_in_coada(void *c, char *nume_referinta, TLG *L, FILE *fp)
{
    TLG p;
    int pozitie=0;
    for(p=*L;p!=NULL;p=p->urm)
    {
        pozitie++;
        Serial *sl=(Serial *)p->info;
        if(strcmp(sl->nume_serial,nume_referinta)==0)
            break;
    }
    if(p==NULL)
        return 0;
    /*Mut conținutul informația într-un alt pointer*/
    void *inf;
    inf=malloc(sizeof(Serial));
    memcpy(inf,p->info,sizeof(Serial));
    if(p==*L)
        evita_primul_element(L);
    else
    {
        if(p->urm!=NULL)
            evita_element_interior(*L,pozitie);
        else
            evita_ultimul_element(*L);
    }
    if((IC(c)==NULL)&&(SC(c)==NULL))
        InsCoadavida(c,inf);
    else
        InsCoadanevida(c,inf);
    fprintf(fp,"Serialul %s se afla in coada de asteptare pe pozitia %d.\n",nume_referinta,numara_elemente(c));
    return 1;
}

/*Funcție care funcționează pe principii asemănătoare, însă păstrează specificul listei top10: serialele dedesubtul
celui pe care îl scoatem urcă în clasament. Funcția scoate un serial din lista top10 și o adaugă în coada
watch_later*/
int adauga_din_top10_in_coada(void *c, char *nume_referinta, TLG *L10, TF schimba, FILE *fp)
{
    TLG p;
    for(p=*L10;p!=NULL;p=p->urm)
    {
        Serial *sl=(Serial *)p->info;
        if(strcmp(sl->nume_serial,nume_referinta)==0)
            {
            break;
            }
    }
    void *inf;
    inf=malloc(sizeof(Serial));
    memcpy(inf,p->info,sizeof(Serial));
    scoate_din_top10(L10,nume_referinta,schimba);
    if((IC(c)==NULL)&&(SC(c)==NULL))
        InsCoadavida(c,inf);
    else
        InsCoadanevida(c,inf);
    fprintf(fp,"Serialul %s se afla in coada de asteptare pe pozitia %d.\n",nume_referinta,numara_elemente(c));
    return 1;
}

/*Funcție care mută un serial din coada watch_later și o adaugă în stiva currently watching. f este o funcție care verifică
dacă numele a două seriale sunt egale*/
int adauga_in_stiva_din_coada(void *s, void *s1, void *c, char *nume_referinta, int timp, TFCmp f, FILE *fp)
{
    TLG p;
    for(p=IC(c);p!=NULL;p=p->urm)
    {
        Serial *sl=(Serial *)p->info;
        if(strcmp(sl->nume_serial,nume_referinta)==0)
            break;
    }
    if(p==NULL)
        return 0;
    /*Prelucrăm informația găsită*/
    Serial *sl=(Serial *)p->info;
    if(sl->durata_vizionare<=timp)
    {
        sl->durata_vizionare=0;
        fprintf(fp,"Serialul %s a fost vizionat integral.\n",nume_referinta);
        if(p==IC(c))
        {
            void *element;
            element=malloc(sizeof(Serial));
            /*Dacă serialul căutat se află în vârful cozii, extragem din coadă și trimitem direct în stiva history*/
            ExtragereCoada(c,element);
            Push(s1,element);
            return 1;
        }
        void *c_aux;
        c_aux=InitQ(DIME(c));
        int pas=0;
        /*În cazul în care serialul căutat nu se află la începutul cozii, se procedează astfel:
        se inițializează o coadă auxiliară în care vor fi păstrate în prima fază serialele situate deasupra
        celui pe care vrem să-l scoatem. În felul acesta serialul țintă va fi adus în vârf. Odată ajuns în vârf,
        este extras și mutat direct în stiva history. În coada auxiliară vor sosi, tot prin extrageri repetate
        celelalte seriale. La final, golim coada auxiliară, timițând restul serialelor la loc, păstrând ordinea.
        În mod similar se procedează și pentru mutarea unui serial în stiva currently watching*/

        /*Cât timp n-am ajuns încă la serial*/
        while(f(IC(c)->info,sl)==0)
        {
            void *element;
            element=malloc(sizeof(Serial));
            ExtragereCoada(c,element);//extragem vârful cozii inițiale
            if(pas==0)
                InsCoadavida(c_aux,element);
            else
                InsCoadanevida(c_aux,element);
            pas++;
            //și punem în coada auxiliară
        }
        void *varf;
        varf=malloc(sizeof(Serial));
        /*Am ajuns în vârf, deci îl extrag și îl pun direct în stivă*/
        ExtragereCoada(c,varf);
        Push(s1,varf);
        while((IC(c)!=NULL)&&(SC(c)!=NULL))//golim coada inițială
        {
            void *element;
            element=malloc(sizeof(Serial));
            ExtragereCoada(c,element);
            InsCoadanevida(c_aux,element);
        }
        pas=0;
        while((IC(c_aux)!=NULL)&&(SC(c_aux)!=NULL))//golim coada auxiliară
        {
            void *element;
            element=malloc(sizeof(Serial));
            ExtragereCoada(c_aux,element);
            if(pas==0)
                InsCoadavida(c,element);
            else
                InsCoadanevida(c,element);
            pas++;
        }
        DistrQ_complexa(&c_aux,stergere_serial);
    }
    else/*În continuare este prezentat același raționament, însă diferă stiva destinație*/
    {
        sl->durata_vizionare=sl->durata_vizionare-timp;
        if(p==IC(c))
        {
            void *cautat;
            cautat=malloc(sizeof(Serial));
            ExtragereCoada(c,cautat);
            Push(s,cautat);
            return 1;
        }
        else
        {
            void *c_aux;
            c_aux=InitQ(DIME(c));
            int pas=0;
            while(f(IC(c)->info,sl)==0)
            {
                void *element;
                element=malloc(sizeof(Serial));
                ExtragereCoada(c,element);
                if(pas==0)
                    InsCoadavida(c_aux,element);
                else
                    InsCoadanevida(c_aux,element);
                pas++;
            }
            void *cautat=malloc(sizeof(Serial));
            ExtragereCoada(c,cautat);
            Push(s,cautat);
            while((IC(c)!=NULL)&&(SC(c)!=NULL))
            {
                void *element;
                element=malloc(sizeof(Serial));
                ExtragereCoada(c,element);
                InsCoadanevida(c_aux,element);
            }
            int pas_inapoi=0;
            while((IC(c_aux)!=NULL)&&(SC(c_aux)!=NULL))
            {
                void *element;
                element=malloc(sizeof(Serial));
                ExtragereCoada(c_aux,element);
                if(pas_inapoi==0)
                    InsCoadavida(c,element);
                else
                    InsCoadanevida(c,element);
                pas_inapoi++;

            }
            DistrQ_complexa(&c_aux,stergere_serial);

        }
    }
    return 1;
}
/*Funcție care scoate un serial din top10 și îl introduce în stivă. Primește și o funcție care schimbă poziția serialelor, care,
inițial erau sub serialul pe care doream să-l scoatem.*/
int adauga_in_stiva_din_top10(void *s, void *s1, TLG *L10, char *nume_referinta, int timp, TF schimba, FILE *fp)
{
    TLG p;
    for(p=*L10;p!=NULL;p=p->urm)
    {
        Serial *sl=(Serial *)p->info;
        if(strcmp(sl->nume_serial,nume_referinta)==0)
            break;
    }
    if(p==NULL)
        return 0;
    scoate_din_top10(L10,nume_referinta,schimba);
   Serial *sl1=(Serial *)p->info;
   //printf("%d\n",timp);
   if(timp>=sl1->durata_vizionare)
   {
        sl1->durata_vizionare=0;
        void *inf=malloc(sizeof(Serial));
        memcpy(inf,p->info,sizeof(Serial));
        Push(s1,inf);
        fprintf(fp,"Serialul %s a fost vizionat integral.\n",nume_referinta);
   }
   else
            {
            sl1->durata_vizionare=sl1->durata_vizionare-timp;
            void *inf=malloc(sizeof(Serial));
            memcpy(inf,p->info,sizeof(Serial));
            Push(s,inf);
            }

    return 1;
}

/*Funcție care localizează un serial într-o coadă și adaugă un sezon*/
int adauga_sezon_serial_coada(void *c, char *nume_referinta, FILE *fp)
{
    TLG p;
    for(p=IC(c);p!=NULL;p=p->urm)
    {
        Serial *sl=(Serial *)p->info;
        if(strcmp(sl->nume_serial,nume_referinta)==0)
            break;
    }
    if(p==NULL)
        return 0;
    Serial *sl1=(Serial *)p->info;
    (sl1->nr_sezoane)++;
    Sezon *sz=(Sezon *)malloc(sizeof(Sezon));
    if(sz==NULL)
        return 0;
    char *nr_episoade_string=strtok(NULL," ");
    int nr_episoade_1=atoi(nr_episoade_string);
    sz->nr_episoade=nr_episoade_1;
    sz->coada_episoadelor=InitQ(sizeof(int));
    int i;
    int *v=(int*)malloc(nr_episoade_1*sizeof(int));
    for(i=0;i<nr_episoade_1;i++)
        {
        char *durata_episod_string=strtok(NULL," ");
        v[i]=atoi(durata_episod_string);
        (sl1->durata_vizionare)=(sl1->durata_vizionare)+v[i];
        if(i==0)
            InsCoadavida(sz->coada_episoadelor,&v[i]);
        else
            InsCoadanevida(sz->coada_episoadelor,&v[i]);
        }
    InsCoadanevida(sl1->coada_sezoanelor,sz);
    fprintf(fp,"Serialul %s are un sezon nou.\n",nume_referinta);
    return 1;

}
/*Funcție care adaugă un sezon pentru un serial aflat într-o stivă.*/
int adauga_sezon_serial_stiva(void *s, char *nume_referinta, FILE *fp)
{
    TLG p;
    for(p=VF(s);p!=NULL;p=p->urm)
    {
        Serial *sl=(Serial *)p->info;
        if(strcmp(sl->nume_serial,nume_referinta)==0)
            break;
    }
    Serial *sl1=(Serial *)p->info;
    (sl1->nr_sezoane)++;
    Sezon *sz=(Sezon *)malloc(sizeof(Sezon));
    if(sz==NULL)
        return 0;
    char *nr_episoade_string=strtok(NULL," ");
    int nr_episoade_1=atoi(nr_episoade_string);
    sz->nr_episoade=nr_episoade_1;
    sz->coada_episoadelor=InitQ(sizeof(int));
    int i;
    int *v=(int*)malloc(nr_episoade_1*sizeof(int));
    for(i=0;i<nr_episoade_1;i++)
        {
        char *durata_episod_string=strtok(NULL," ");
        v[i]=atoi(durata_episod_string);
        (sl1->durata_vizionare)=(sl1->durata_vizionare)+v[i];
        if(i==0)
            InsCoadavida(sz->coada_episoadelor,&v[i]);
        else
            InsCoadanevida(sz->coada_episoadelor,&v[i]);
        }
    InsCoadanevida(sl1->coada_sezoanelor,sz);
    fprintf(fp,"Serialul %s are un sezon nou.\n",nume_referinta);
    return 1;
}
