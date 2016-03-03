#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "datacheck.h"

#define BUFF_SIZE 35

static int checkClient(char *line, CATALOG cat, CATALOG nil);
static int checkProduct(char *line, CATALOG cat, CATALOG nil);
static int checkSale(char *line, CATALOG productCat, CATALOG clientCat);

int checkFile (FILE *file, CATALOG cat1, CATALOG cat2, int mode, int *sucLn, int *failLn) {
	int checked_line, suc, fail;
	char buf[BUFF_SIZE], *line;
	int (*checker)(char *, CATALOG, CATALOG);

	suc = fail = 0;

	switch(mode) {
		case M_CLIENTS: checker = checkClient; break;
		case M_PRODUCTS: checker = checkProduct; break;
		case M_SALES: checker = checkSale; break;
	}

	while(fgets(buf, BUFF_SIZE, file)) {
		line = strtok (buf, "\n\r");
		checked_line = checker(line, cat1, cat2);

		if (checked_line) {
			suc++;
			if (mode != M_SALES) insert(cat1, line);
		} else fail++;
	}

	*sucLn = suc;
	*failLn = fail;

	return 0;
}

static int checkProduct (char *line, CATALOG nil1, CATALOG nil2){
	int i, lnOk;

	lnOk = 1;

	for (i = 0; lnOk && i < 6; i++){
		switch (i){
			case 0:
			case 1: lnOk = (line[i] >= 'A' && line[i] <= 'Z');
					break;
			case 2: lnOk = (line[i] == '1');
					break;
			case 3:
			case 4:
			case 5: lnOk = (line[i] >= '0' && line[i] <= '9');
					break;
			case 6: lnOk = (line[i] == '\n');
		}
	}

	return lnOk;
}

static int checkClient (char *line, CATALOG nil1, CATALOG nil2) {
	int i, lnOk;

	lnOk = 1;

	for (i = 0; lnOk && i < 6; i++){
		switch (i){
			case 0: lnOk = (line[i] >= 'A' && line[i] <= 'Z');
					break;
			case 1: lnOk = (line[i] >= '1' && line[i] <= '5');
					break;
			case 2:
			case 3:
			case 4: lnOk = (line[1] == '5' && line[i] == '0') ||
							   (line[1] != '5' && line[i] >= '0' && line[i] <= '9');
					break;
		}
	}

	return lnOk;
}

static int checkSale (char *line, CATALOG productCat, CATALOG clientCat) {
	int i, lnOk, quant, month, filial;
	double price;
	char *token;

	lnOk = 1;
	token = strtok(line, " ");

	for (i = 0; lnOk && token != NULL; i++){
		switch(i) {
			case 0: lnOk = checkProduct(token, NULL, NULL) && (lookUp(productCat, token) != -1);
						break;
			case 1: lnOk = ((price = atof(token)) >= 0 && price <= 999.99);
						break;
			case 2: lnOk = ((quant = atoi(token)) >= 1 && quant <= 200);
						break;
			case 3: lnOk = !strcmp(token, "P") || !strcmp(token, "N");
						break;
			case 4: lnOk = checkClient(token, NULL, NULL) && (lookUp(clientCat, token) != -1);
						break;
			case 5: lnOk = (month = atoi(token) >= 1 && month < 12);
						break;
			case 6: lnOk = ((filial = atoi(token)) >= 1 && filial <= 3);
						break;
			default: lnOk = 0;
		}
		token = strtok(NULL, " ");
	}

	return lnOk;
}
