#include <stdlib.h>
#include <stdio.h>
#include "interpreter.h"
#include "clients.h"
#include "products.h"
#define LINES_NUM 20

void present(PRODUCTSET ps) {
	char *str, nav='\0'; 
	int i, cpage = 1, totalPages, cont=0, total = getPSetSize(ps) ;
	PRODUCT p;

	totalPages = (total / LINES_NUM) + (total % LINES_NUM != 0);

	while (cpage <= totalPages) {
		
		printf("==========( Página %d de %d )==========\n", cpage, totalPages);
	
		for (i=0; i < LINES_NUM && cont < total; cont++, i++) {
			p = getPSetData(ps, cont);

			str = fromProduct(p);

			printf("\t\t%s\n", str);

			freeProduct(p);
			free(str);
		}

		printf("<----(b)---- (=============) --(enter)-->\n");	

		while(nav != 'b' && nav != 'q' && nav != '\n') {
			nav = getchar();
		}
		if (nav == 'q') break;	

		cpage = (nav == '\n') ? cpage+1 : cpage-1;
	}
}
