/*VÎRLAN Mihnea-Alexandru - 311CB*/
#include "TCoada.h"
#include "tlg.h"

#ifndef _COADA_LISTE_
#define _COADA_LISTE_

typedef struct
{
	size_t 	dime; 		/* dimensiunea unui element din coada  		*/
	TLG ic,sc;
} TCoada, *AQ;

/*- macrodefinitii - acces campuri -*/
#define IC(a) 	(((AQ)(a))->ic)
#define SC(a) 	(((AQ)(a))->sc)
#define DIMEc(a) (((AQ)(a))->dime)


#endif
