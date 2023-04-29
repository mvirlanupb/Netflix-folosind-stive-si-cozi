/*VÎRLAN Mihnea-Alexandru - 311CB*/
/*-- TStiva-Liste.h -- elementele stivei sunt memorate într-o listă --*/
#include "TStiva.h"
#include "tlg.h"

#ifndef _STIVA_LISTE_
#define _STIVA_LISTE_

typedef struct stiva
{
	size_t 	dime; 		/* dimensiunea unui element al stivei 			*/
	TLG vf;          /*Adresa celulei din varf */
} TStiva, *ASt;

/*- macrodefinitii - acces campuri */
#define DIME(a) (((ASt)(a))->dime)
#define VF(a) 	(((ASt)(a))->vf)

#endif
