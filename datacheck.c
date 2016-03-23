#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "datacheck.h"

#define BUFF_SIZE 35
#define MAX_CATALOG 10000

static int checkClient(char *line);
static int checkProduct(char *line);
static int checkSaleLn(char *line, CATALOG product, CATALOG client);

void writeCatalog (FILE *file, CATALOG cat, int mode, int *sucLn, int *failLn) {
	int checked_line, suc, fail;
	char buf[BUFF_SIZE], *line;
	int (*checker)(char *) = NULL;

	suc = fail = 0;

	switch(mode) {
		case M_CLIENTS: checker = checkClient; break;
		case M_PRODUCTS: checker = checkProduct; break;
	}

	while(fgets(buf, BUFF_SIZE, file)) {
		line = strtok (buf, "\n\r");
		checked_line = checker(line);

		if (checked_line) {
			insert(cat, line);
			suc++;
		} else
			fail++;
	}

	*sucLn = suc;
	*failLn = fail;
}

int checkSales (FILE *file, CATALOG products, CATALOG clients, int *sucLn, int *failLn) {
	int checked_line, suc, fail;
	char buf[BUFF_SIZE], *line, print[BUFF_SIZE];
	FILE * validSales = fopen ("Vendas_1MValidas.txt", "w");

	suc = fail = 0;

//temos que verificar se file existe
	while(fgets(buf, BUFF_SIZE, file)) {
		line = strtok (buf, "\n\r");
		strcpy(print, line);
		checked_line = checkSaleLn(line, products, clients);

		/*(checked_line) ? suc++ : fail++; */
		if (checked_line) {
			fprintf(validSales, "%s\n", print);
			suc++;
		} else
			fail++;
	}

	*sucLn = suc;
	*failLn = fail;

	fclose(validSales);
	return 0;
}

static int checkProduct (char *line){
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

static int checkClient (char *line) {
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

static int checkSaleLn (char *line, CATALOG productCat, CATALOG clientCat) {
	int i, lnOk, quant, month, filial;
	double price;
	char *token;

	lnOk = 1;
	token = strtok(line, " ");

	for (i = 0; lnOk && token != NULL; i++){
		switch(i) {
			case 0: lnOk = lookUp(productCat, token);
						break;
			case 1: lnOk = ((price = atof(token)) >= 0 && price <= 999.99);
						break;
			case 2: lnOk = ((quant = atoi(token)) >= 1 && quant <= 200);
						break;
			case 3: lnOk = !strcmp(token, "P") || !strcmp(token, "N");
						break;
			case 4: lnOk = lookUp(clientCat, token);
						break;
			case 5: lnOk = (month = atoi(token) >= 1 && month <= 12);
						break;
			case 6: lnOk = ((filial = atoi(token)) >= 1 && filial <= 3);
						break;
			default: lnOk = 0;
		}
		token = strtok(NULL, " ");
	}

	return lnOk;
}


void testsValidSales() {

    int i, priceIsZero=0, totalQuant=0, quantity=0, totalClients=0, totalProducts=0;
    float price, billed=0;
    char line[BUFF_SIZE], *token;
    FILE * file = fopen("Vendas_1MValidas.txt", "r");

    CATALOG clients, products;

    clients = initCatalog();
    products = initCatalog();

    while (fle  & fgets(line, BUFF_SIZE, file)) {

        token = strtok(line, " \n\r");

        for (i = 0; token != NULL; i++){
    		switch(i) {
    			case 0: if (!lookUp(products, token)) {
                            totalProducts++;
                            insert(products, token);
                        }
                        break;
    			case 1: price = atof(token);
                        if (!price) priceIsZero++;
                        billed+=price;
    					break;
    			case 2: quantity = atoi(token);
                        totalQuant += quantity;
    					break;
    			case 3: /* N/P */
    					break;
    			case 4: if (!lookUp(clients, token)) {
                            totalClients++;
                            insert(clients, token);
                        }
    					break;
    			case 5: /* Month */
    					break;
    			case 6: /* Filial */
    					break;
    		}
    		token = strtok(NULL, " ");
    	}
    }

    printf("Preço Zero: %d\n", priceIsZero);
    printf("Total Faturado: %f\n", billed);
    printf("Unidades Vendidas: %d\n", totalQuant);NF1153
    printf("Total Clientes: %d\n", totalClients);
    printf("Total Produtos: %d\n", totalProducts);
}
