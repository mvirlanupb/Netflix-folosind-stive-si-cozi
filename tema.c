/*VÎRLAN Mihnea-Alexandru - 311CB*/
#include "TCoada.h"
#include "TCoada-Liste.h"
#include "Seriale.h"
#include "TStiva-liste.h"
#include "TStiva.h"


int comparare_rating(void *a, void *b)
{
    Serial *sl1=(Serial*)a;
    Serial *sl2=(Serial*)b;
    if(sl1->rating>sl2->rating)
        return 1;
    if(sl1->rating==sl2->rating)
        return 0;
    return -1;

}

int comparare_nume(void *a, void *b)
{
    Serial *sl1=(Serial*)a;
    Serial *sl2=(Serial*)b;
    if(strcmp(sl1->nume_serial,sl2->nume_serial)>0)
        return 1;
    return 0;
}

int extrage_pozitie_top10(void *a)
{
    Serial *sl=(Serial*)a;
    return sl->pozitie_top;
}

void actualizare_top(void *a)
{
    Serial *sl=(Serial *)a;
    (sl->pozitie_top)++;
}

void scade_top(void *a)
{
    Serial *sl=(Serial *)a;
    (sl->pozitie_top)--;
}

int egalitate_seriale(void *a, void *b)
{
    Serial *sl1=(Serial *)a;
    Serial *sl2=(Serial *)b;
    if(strcmp(sl1->nume_serial,sl2->nume_serial)==0)
        return 1;
    return 0;

}

int main(int argv, char **argc)
{
    FILE *fp1,*fp2;
    fp1=fopen(argc[1],"r");
    fp2=fopen(argc[2],"w");
    size_t lungime=0;
    char *linie,*comanda,id,*nume_referinta,*categorie;
    TLG ID1,ID2,ID3,TOP10;
    void *watch_later,*currently_watching, *history;
    currently_watching=InitS(sizeof(Serial));
    history=InitS(sizeof(Serial));
    watch_later=InitQ(sizeof(Serial));
    nume_referinta=(char*)malloc(33*sizeof(char));
    ID1=NULL;
    ID2=NULL;
    ID3=NULL;
    TOP10=NULL;
    linie=NULL;
    while(getline(&linie,&lungime,fp1)!=-1)
        {
            if((linie[0]=='a')&&(linie[1]=='d')&&(linie[2]=='d')&&(linie[3]==' '))
                id=linie[4];
            comanda=strtok(linie," ");
            if(strcmp(comanda,"add")==0)
                {
                    if(id=='1')
                        citire_serial(&ID1,comparare_rating,comparare_nume,fp2);
                    if(id=='2')
                        citire_serial(&ID2,comparare_rating,comparare_nume,fp2);
                    if(id=='3')
                        citire_serial(&ID3,comparare_rating,comparare_nume,fp2);
                }
            if(strcmp(comanda,"add_sez")==0)
                {
                    int rez;
                    nume_referinta=strtok(NULL," ");
                    rez=adauga_sezon_serial_lista(ID1,nume_referinta,fp2);
                    if(rez==0)
                        {
                            rez=adauga_sezon_serial_lista(ID2,nume_referinta,fp2);
                            if(rez==0)
                                {
                                    rez=adauga_sezon_serial_lista(ID3,nume_referinta,fp2);
                                    if(rez==0)
                                        {
                                        rez=adauga_sezon_serial_lista(TOP10,nume_referinta,fp2);
                                        if(rez==0)
                                        {
                                            rez=adauga_sezon_serial_coada(watch_later,nume_referinta,fp2);
                                            if(rez==0)
                                                rez=adauga_sezon_serial_stiva(currently_watching,nume_referinta,fp2);
                                        }
                                        }
                                }
                        }
                }

                if(strcmp(comanda,"add_top")==0)
                        citire_serial_top10(&TOP10,extrage_pozitie_top10,actualizare_top,stergere_serial,fp2);
                if(strcmp(comanda,"watch")==0)
                        {
                        nume_referinta=strtok(NULL," ");
                        char *timp_ales=strtok(NULL," ");
                        if(timp_ales[strlen(timp_ales)-1]=='\n')
                            timp_ales[strlen(timp_ales)-1]='\0';
                        int timp=atoi(timp_ales);
                        int rez1;
                        rez1=actualizeaza_in_stiva(currently_watching,history,nume_referinta,timp,egalitate_seriale,fp2);
                        if(rez1==0)
                            {
                            rez1=adaugare_in_stiva_watching_din_lista(currently_watching,history,nume_referinta,timp,&ID1,fp2);
                            if(rez1==0)
                                {
                                rez1=adaugare_in_stiva_watching_din_lista(currently_watching,history,nume_referinta,timp,&ID2,fp2);
                                if(rez1==0)
                                    {
                                    rez1=adaugare_in_stiva_watching_din_lista(currently_watching,history,nume_referinta,timp,&ID3,fp2);
                                    if(rez1==0)
                                        {
                                        rez1=adauga_in_stiva_din_coada(currently_watching,history,watch_later,nume_referinta,timp,egalitate_seriale,fp2);
                                        if(rez1==0)
                                            rez1=adauga_in_stiva_din_top10(currently_watching,history,&TOP10,nume_referinta,timp,scade_top,fp2);
                                        }
                                    }
                            }

                            }
                        }
                if(strcmp(comanda,"later")==0)
                            {
                            nume_referinta=strtok(NULL," ");
                            if(nume_referinta[strlen(nume_referinta)-1]=='\n')
                                nume_referinta[strlen(nume_referinta)-1]='\0';
                            int rez;
                            rez=adauga_serial_in_coada(watch_later,nume_referinta,&ID1,fp2);
                            if(rez==0)
                                {
                                rez=adauga_serial_in_coada(watch_later,nume_referinta,&ID2,fp2);
                                if(rez==0)
                                    {
                                    rez=adauga_serial_in_coada(watch_later,nume_referinta,&ID3,fp2);
                                    if(rez==0)
                                        rez=adauga_din_top10_in_coada(watch_later,nume_referinta,&TOP10,scade_top,fp2);
                                    }
                                }
                            }
                if(strcmp(comanda,"show")==0)
                                {
                                categorie=strtok(NULL," ");
                                if(categorie[strlen(categorie)-1]=='\n')
                                    categorie[strlen(categorie)-1]='\0';
                                fprintf(fp2,"Categoria %s: ",categorie);
                                if(strcmp(categorie,"1")==0)
                                    Afisare(&ID1,afisare_serial,fp2);
                                if(strcmp(categorie,"2")==0)
                                    Afisare(&ID2,afisare_serial,fp2);
                                if(strcmp(categorie,"3")==0)
                                    Afisare(&ID3,afisare_serial,fp2);
                                if(strcmp(categorie,"later")==0)
                                    AfisareCoada(watch_later,afisare_serial,fp2);
                                if(strcmp(categorie,"watching")==0)
                                    Afiseaza_Stiva(currently_watching,afisare_serial,fp2);
                                if(strcmp(categorie,"history")==0)
                                    Afiseaza_Stiva(history,afisare_serial,fp2);
                                if(strcmp(categorie,"top10")==0)
                                    Afisare(&TOP10,afisare_serial,fp2);
                                }

            }
    Distruge(&ID1,stergere_serial);
    Distruge(&ID2,stergere_serial);
    Distruge(&ID3,stergere_serial);
    Distruge(&TOP10,stergere_serial);
    DistrQ_complexa(&watch_later,stergere_serial);
    DistrS(&currently_watching,stergere_serial);
    DistrS(&history,stergere_serial);
    fclose(fp1);
    fclose(fp2);

    return 0;
}
