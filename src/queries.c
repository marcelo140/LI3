#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queries.h"
#include <time.h>
#include "set.h"
#define UPPER(a) (('a' <= (a) && (a) <= 'z') ? ((a - 'a') + 'A') : (a))
#define MAX_SIZE 128
#define BRANCHES 3
#define NP 2

PRINTSET query2(PRODUCTCAT pcat, char index) {
	PRINTSET print;
	LIST l;
	char* product;
	int i, size;
	char aux[MAX_SIZE];

	size  = countProducts(pcat, index);
	print = initPrintSet(size);
	
	l = fillProductSet(pcat, index);

	product = getListElement(l, 0);
	for(i=0; product ; i++) {

		sprintf(aux, "\t\t\t%s", product);
		print = addToPrintSet(print, aux);
		product = getListElement(l,i+1);
	}

	freeList(l);

	return print;
}

PRINTSET query3(FATGLOBAL fat, PRODUCT product, int month) {
	PRINTSET print = initPrintSet(10);
	PRODUCTFAT pfat;
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

	pfat = getProductDataByMonth(fat, product, month);

	if (mode == 1) {
		quantT  = 0;
		billedT = 0;
		for (i=0; i < BRANCHES; i++) {
			quantT  += getProductFatQuant(pfat, i, NULL, NULL);
	   		billedT += getProductFatQuant(pfat, i, NULL, NULL); 		
		}	
		sprintf(answ, "Quantidade Total: \t%d", quantT);
		print = addToPrintSet(print, answ);	
		sprintf(answ, "Faturação Total: \t%.2f", billedT);
		print = addToPrintSet(print, answ);
	} else {
		print = setPrintHeader(print, "\t\tFilial 1\tFilial 2\tFilial 3");
		print = addToPrintSet(print, "");
	
		for(i=0; i < BRANCHES; i++) { 
			getProductFatQuant(pfat, i, &quantity[i][0], &quantity[i][1]);
			getProductFatBilled(pfat, i, &billed[i][0], &billed[i][1]);
		}

		sprintf(answ, "Quantidade N\t %d\t\t %d\t\t %d", quantity[0][0], quantity[1][0], quantity[2][0]);
		print = addToPrintSet(print, answ);
		sprintf(answ, "Quantidade P\t %d\t\t %d\t\t %d", quantity[0][1], quantity[1][1], quantity[2][1]);
		print = addToPrintSet(print, answ);
	
		print = addToPrintSet(print, "");

		sprintf(answ, "Faturado N\t %.2f\t\t %.2f\t\t %.2f", billed[0][0], billed[1][0], billed[2][0]);
		print = addToPrintSet(print, answ);
		sprintf(answ, "Faturado P\t %.2f\t\t %.2f\t\t %.2f", billed[0][1], billed[1][1], billed[2][1]);
		print = addToPrintSet(print, answ);
	}

	freeProductFat(pfat);

	return print;
}

PRINTSET query4(FATGLOBAL fat) {
	PRINTSET print = initPrintSet(MAX_SIZE);
	LIST *pgroupB, pgroupT;
	char buff[MAX_SIZE], *product;
	int i, j, op, n=0;

	printf("\n  ::::::::::::::::::::::::::::::\n\n");
	printf("   1• Total\n");
	printf("   2• Todas as Filiais\n");
	printf("\n  ::::::::::::::::::::::::::::::\n");
	
	while(1) {
		printf("  Escolha um modo: ");
		fgets(buff, MAX_SIZE, stdin);
		op = atoi(buff);
		if (op == 1 || op == 2) break;
		if (UPPER(buff[0]) == 'Q') return NULL;
	    printf("Por favor escolha entre 1 e 2.\n");	
	}

	if (op == 1) {
		pgroupT = getProductsNotSold(fat);
		product = getListElement(pgroupT, 0);

		for (i=0; product ; i++) {
			sprintf(buff, "\t\t\t%s", product);
			print = addToPrintSet(print, buff);
			product = getListElement(pgroupT, i+1);
		}

/*		freeProductGroup(pgroupT); */
	} else {
		pgroupB = getProductsNotSoldByBranch(fat);
		
		sprintf(buff, "\tFilial 1\tFilial 2\tFilial 3");
		print = setPrintHeader(print, buff);

		for (i=0;  ; i++){
			n = 0;
			buff[0] = '\0';
			for (j=0; j < BRANCHES; j++) {
				product = getListElement(pgroupB[j], i);
				if (product) sprintf(buff, "%s\t%s", buff, product);
				else { sprintf(buff,"%s\t\t", buff); n++;}
			}
			if (n==3) break;
			print = addToPrintSet(print, buff);
		}
		
	}

	return print;
}

PRINTSET query5(BRANCHSALES bs, CLIENT client) {
	PRINTSET print = initPrintSet(12);
	char str[MAX_SIZE];
	int i, *quantity;

	quantity = getClientQuant(bs, client);

	sprintf(str, "\tMÊS\t    QUANTIDADE");
	print = setPrintHeader(print, str);

	for(i=0; i < 12; i++){
		sprintf(str, "\t%2d\t\t%3d", i+1, quantity[i]);
		print = addToPrintSet(print, str);
	}

	free(quantity);
	return print;
}

PRINTSET query6(FATGLOBAL fat, int initialMonth, int finalMonth) {
	PRINTSET print = initPrintSet(2);
	double billed;
	int quantity;
	char buff[MAX_SIZE];

	quantity = getQuantByMonthRange(fat, initialMonth, finalMonth);
	billed   = getBilledByMonthRange(fat, initialMonth, finalMonth);
	
	sprintf(buff, "Quantidade:\t%d", quantity);
	print = addToPrintSet(print, buff);
	sprintf(buff, "Faturado:\t%.2f", billed);
	print = addToPrintSet(print, buff);

	return print;
}

PRINTSET query8(BRANCHSALES bs, PRODUCT product){
	PRINTSET print = NULL;
	SET n = initSet(1024), p = initSet(1024), toPrint;
	char answ[MAX_SIZE], *buff;
	int mode = 0, i;

	filterClientsByProduct(bs, product, n, p);
	
	printf("\n::::::::::::::::::::::::::::::\n\n");
	printf(" 1• Clientes em modo N (%d)\n", getSetSize(n));
	printf(" 2• Clientes em modo P (%d)\n", getSetSize(p));
	printf("\n::::::::::::::::::::::::::::::\n");

	while (mode <= 0 || mode >= 3) { 
		printf("Escolha um modo: ");
		fgets(answ, MAX_SIZE, stdin);
		mode = atoi(answ);
		
		if (answ[0] == 'q') break; 
		
		if (UPPER(answ[0]) == 'N')
			mode = 1;
		else if (UPPER(answ[0]) == 'P')
			mode = 2;
	}
	
	if (mode == 1 || mode == 2) {
		toPrint = (mode == 1) ? n : p;
		print = initPrintSet(MAX_SIZE);
		
		for(i=0; (buff = getSetHash(toPrint, i)) ; i++) {
			sprintf(answ, "\t\t%s",buff);
			print = addToPrintSet(print, answ);
			free(buff);
		}
	}

	freeSet(n);
	freeSet(p);

	return print;
}

PRINTSET query10(BRANCHSALES bs, int n) {
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

	sprintf(buff, "\t\tPRODUTO\t C    Q");
	print = setPrintHeader(print, buff);

	for(i=0; i < n ; i++) {
		product  = getNameFromProductData(pdata[i]);
		clients  = getClientsFromProductData(pdata[i]);
		quantity = getQuantFromProductData(pdata[i]);
		
		sprintf(buff, "%5dº \t%s\t%2d  %4d", i+1, product, clients, quantity );
		print = addToPrintSet(print, buff);

		free(product);
	}

	
	fim = clock();
	tempo = (double) (fim - inicio) / CLOCKS_PER_SEC; 

	printf("Tempo: %f\n", tempo);

	return print;	
} 
