#include <stdlib.h>
#include <stdio.h>
#include "interpreter.h"
#include "clients.h"

#define LINES_NUM 20

void present(PRODUCTSET ps) {
	char *str, nav='\0', onav;
	int i, cpag = 0, totalPags= psetSize(ps);
	PRODUCT p;

 
	totalPags = (totalPags / LINES_NUM); /* + (totalPags % LINES_NUM != 0); */

	while (cpag <= totalPags) {
		
		system("clear");
		printf("==========( Página %d de %d )==========\n", cpag, totalPags);

	
		for (i=0; i < LINES_NUM; i++) {
			p = getPsetData(ps, cpag*LINES_NUM + i);

			if (!p) break; 

			str = fromProduct(p);

			printf("\t%s\n", str);

			freeProduct(p);
			free(str);
		}

		printf(" <--(b)-- (=================) --(n)--> \n");	

		while(1) {
			onav = nav;
			nav = getchar();
		
			if (nav == '\n') nav = onav;	
			if (nav == 'b' && cpag > 0) { cpag--; break; }
			if (nav == 'n') { cpag++; break; }
			if (nav == 'q') {break;}
		}

		if (nav == 'q') break;	
	}
}
