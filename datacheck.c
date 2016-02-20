#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "datacheck.h"

#define BUFF_SIZE 35

static int checkClient(char *line);
static int checkProduct(char *line);
static int checkSale(char *line);

int checkFile(FILE *file, int mode, int *sucLn, int *failLn) {
	int checked_line, suc, fail;
	char buf[BUFF_SIZE];
	int (*checker)(char *);

	suc = fail = 0;

	switch(mode) {
		case M_CLIENTS: checker = checkClient; break;
		case M_PRODUCTS: checker = checkProduct; break;
		case M_SALES: checker = checkSale; break;
		default: return -1;
	}

	while(fgets(buf, BUFF_SIZE, file)) {
		checked_line = checker(buf);
		(checked_line) ? suc++ : fail++;
	}

	*sucLn = suc;
	*failLn = fail;

	return 0;
}

static int checkProduct (char *line){
	int i, lnFailed;

	lnFailed = 0;

	for (i = 0; !lnFailed && i < 6; i++){
		switch (i){
			case 0:
			case 1: lnFailed = (line[i] < 'A' || line[i] > 'Z');
					break;
			case 2: lnFailed = (line[i] != '1');
					break;
			case 3:
			case 4:
			case 5: lnFailed = (line[i] < '0' || line[i] > '9');
					break;
			case 6: lnFailed = (line[i] != '\n');
		}
	}

	return !lnFailed;
}

static int checkClient (char *line) {
	int i, lnFailed;

	lnFailed = 0;

	for (i = 0; !lnFailed && i < 6; i++){
		switch (i){
			case 0: lnFailed = (line[i] < 'A' || line[i] > 'Z');
					break;
			case 1: lnFailed = (line[i] < '1' || line[i] > '5');
					break;
			case 2:
			case 3:
			case 4: lnFailed = (line[1] == '5' && line[i] != '0') ||
							   (line[i] < '0' || line[i] > '9');
					break;
		}
	}

	return !lnFailed;
}

static int checkSale (char *line) {
	int i, lnOk, quant, month, filial;
	float price;
	char *token;

	lnOk = 1;
	token = strtok(line, " \n\r");

	for (i = 0; lnOk && token != NULL; i++){
		switch(i) {
			case 0: lnOk = checkProduct(token);
							break;
			case 1: lnOk = ((price = strtof(token, NULL)) >= 0 && price <= 999.99);
							break;
			case 2: lnOk = ((quant = atoi(token)) >= 1 && quant <= 200);
							break;
			case 3: lnOk = !strcmp(token, "P") || !strcmp(token, "N");
							break;
			case 4: lnOk = checkClient(token);
							break;
			case 5: lnOk = (month = atoi(token) >= 1 && month < 12);
							break;
			case 6: lnOk = ((filial = atoi(token)) >= 1 && filial <= 3);
							break;
			default: lnOk = 0;
		}
		token = strtok(NULL, " \n\r");
	}

	return lnOk;
}
