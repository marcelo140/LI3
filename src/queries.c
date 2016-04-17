#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queries.h"
#include <time.h>

#define UPPER(a) (('a' <= (a) && (a) <= 'z') ? ((a - 'a') + 'A') : (a))
#define MAX_SIZE 128
#define BRANCHES 3
#define NP 2

PRINTSET query2(PRODUCTCAT pcat, char index) {
	PRINTSET print;
	PRODUCTSET ps;
	PRODUCT product;
	int i, size;
	char buff[MAX_SIZE], aux[MAX_SIZE];

	size  = countProducts(pcat, index);
	print = initPrintSet(size);
	ps    = initProductSet(size);
	
	ps = fillProductSet(pcat, ps, index);

	for(i=0; (product = getProductByPos(ps, i)) ; i++) {
		fromProduct(product, buff);
		sprintf(aux, "\t\t\t%s", buff);
		print = addToPrintSet(print, aux);
		freeProduct(product);
	}

	return print;
}

PRINTSET query3(FATGLOBAL fat, PRODUCT product, int month) {
	PRINTSET print = initPrintSet(10);
	char answ[MAX_SIZE];
	int i, mode=0, quantT, quantity[BRANCHES][NP];
	double billedT, billed[BRANCHES][NP];	

	printf("\n::::::::::::::::::::::::::::::\n\n");
	printf(" 1• Total\n");
	printf(" 2• Filial a Filial\n");
	printf("\n::::::::::::::::::::::::::::::\n");

	while(mode != 1 && mode != 2) {
		printf("  Escolha um modo: ");
		fgets(answ, MAX_SIZE, stdin);
		mode = atoi(answ);
		if (UPPER(answ[0]) == 'Q') return NULL;
	}

	getProductDataByMonth(fat, product, month, billed, quantity);

	if (mode == 1) {
		quantT  = 0;
		billedT = 0;
		for (i=0; i < BRANCHES; i++) {
			quantT += quantity[i][0];
			quantT += quantity[i][1];
			billedT += billed[i][0];
			billedT += billed[i][1];
		}	
		sprintf(answ, "Quantidade Total: \t%d", quantT);
		print = addToPrintSet(print, answ);	
		sprintf(answ, "Faturação Total: \t%f", billedT);
		print = addToPrintSet(print, answ);
	} else {
		print = setPrintHeader(print, "\t\tFilial 1\tFilial 2\tFilial 3");
		print = addToPrintSet(print, "");
		
		sprintf(answ, "Quantidade N:\t%d\t\t%d\t\t%d", quantity[0][0], quantity[1][0], quantity[2][0]);
		print = addToPrintSet(print, answ);
		sprintf(answ, "Quantidade P:\t%d\t\t%d\t\t%d", quantity[0][1], quantity[1][1], quantity[2][1]);
		print = addToPrintSet(print, answ);
	
		print = addToPrintSet(print, "");

		sprintf(answ, "Faturado N:\t%f\t%f\t%f", billed[0][0], billed[1][0], billed[2][0]);
		print = addToPrintSet(print, answ);
		sprintf(answ, "Faturado P:\t%f\t%f\t%f", billed[0][1], billed[1][1], billed[2][1]);
		print = addToPrintSet(print, answ);
	}

	return print;
}

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

	while ((mode <= 0 || mode >= 3) && UPPER(answ[0]) != 'Q') {
		printf("Escolha um modo: ");
		fgets(answ, MAX_SIZE, stdin);
		mode = atoi(answ);
		
		if (UPPER(answ[0]) == 'N')
			mode = 1;
		else if (UPPER(answ[0]) == 'P')
			mode = 2;
	}
	if (UPPER(answ[0]) == 'Q') return NULL;
	
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

PRINTSET query10(FATGLOBAL fat, BRANCHSALES bs, int n) {
	PRINTSET print = initPrintSet(n);
	PRODUCTDATA *pdata;  
	char buff[MAX_SIZE], *product;
	int i, clients, quantity;
	int max;

	clock_t inicio, fim;
	double tempo;

	inicio = clock();

	pdata = getAllContent(bs, &max);

	if (max < n)
		n = max;

	sprintf(buff, "\tPRODUTO\tCLIENTES\tQUANTIDADE");
	print = setPrintHeader(print, buff);

	for(i=0; i < n ; i++) {
		product  = getNameFromProductData(pdata[i]);
		clients  = getClientsFromProductData(pdata[i]);
		quantity = getQuantFromProductData(pdata[i]);
		
		sprintf(buff, "%dº\t%s\t%d\t%d", i, product, clients, quantity );
		print = addToPrintSet(print, buff);

		free(product);
	}

	
	fim = clock();
	tempo = (double) (fim - inicio) / CLOCKS_PER_SEC; 

	printf("Tempo: %f\n", tempo);

	return print;	
} 
