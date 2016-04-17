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

PRINTSET query8(BRANCHSALES bs, PRODUCT product){
	PRINTSET print = initPrintSet(MAX_SIZE);
	CLIENTLIST n = newClientList(), p = newClientList(), toPrint;
	char answ[MAX_SIZE], *buff;
	int mode = 0, i;

	filterClientsByProduct(bs, product, n, p);
	
	printf("\n::::::::::::::::::::::::::::::\n\n");
	printf(" 1• Clientes em modo N (%d)\n", clientListSize(n));
	printf(" 2• Clientes em modo P (%d)\n", clientListSize(p));
	printf("\n::::::::::::::::::::::::::::::\n");

	while (mode <= 0 || mode >= 3) {
		printf("Escolha um modo: ");
		fgets(answ, MAX_SIZE, stdin);
		mode = atoi(answ);
	}

	toPrint = (mode == 1) ? n : p;

	for(i=0; (buff = getClientListPos(toPrint, i)) ; i++) {
		sprintf(answ, "\t\t%s",buff);
		print = addToPrintSet(print, answ);
		free(buff);
	}
	
	freeClientList(n);
	freeClientList(p);
	return print;
}
