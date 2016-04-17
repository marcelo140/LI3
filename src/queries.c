#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queries.h"

#define MAX_SIZE 128

PRINTSET query5(BRANCHSALES bs, CLIENT client) {
	PRINTSET print = initPrintSet(12);
	char str[MAX_SIZE];
	int i, *quantity;

	quantity = getClientQuant(bs, client);

	for(i=0; i < 12; i++){
		sprintf(str, "\tMês %d:\t\t%d", i+1, quantity[i]);
		print = addToPrintSet(print, str);
	}

	free(quantity);
	return print;
}
