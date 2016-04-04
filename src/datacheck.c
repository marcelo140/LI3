#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "datacheck.h"

#define BUFF_SIZE 35
#define MAX_CATALOG 10000

static int checkSaleLn(char *line, PRODUCTCAT product, CLIENTCAT client);

CLIENTCAT writeCCat(FILE *file, CLIENTCAT cat, int *sucLn, int *failLn) {
	int suc, fail;
	char buf[BUFF_SIZE], *line;
	CLIENT client;

	suc = fail = 0;

	while(fgets(buf, BUFF_SIZE, file)) {
		line = strtok (buf, "\n\r");
		client = toClient(line);

		if (client){
			cat = insertClient(cat, client);
			suc++;
		} else
			fail++;
	}

	*sucLn = suc;
	*failLn = fail;

	return cat;
}

PRODUCTCAT writePCat(FILE *file, PRODUCTCAT cat, int *sucLn, int *failLn) {
	int suc, fail;
	char buf[BUFF_SIZE], *line;
	PRODUCT product;

	suc = fail = 0;

	while(fgets(buf, BUFF_SIZE, file)) {
		line = strtok (buf, "\n\r");
		product = toProduct(line);

<<<<<<< HEAD:datacheck.c
		if (checked_line) {
			insertCatalog(cat, line);
=======
		if (product) {
			cat = insertProduct(cat, product);
>>>>>>> master:src/datacheck.c
			suc++;
		} else
			fail++;
	}

	*sucLn = suc;
	*failLn = fail;

	return cat;
}

int checkSales (FILE *file, PRODUCTCAT products, CLIENTCAT clients, int *sucLn, int *failLn) {
	int checked_line, suc, fail;
	char buf[BUFF_SIZE], *line, print[BUFF_SIZE];
	FILE * validSales = fopen ("Vendas_1MValidas.txt", "w");

	suc = fail = 0;

/* temos que verificar se file existe */
	while(fgets(buf, BUFF_SIZE, file)) {
		line = strtok (buf, "\n\r");
		strcpy(print, line);
		checked_line = checkSaleLn(line, products, clients);

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

static int checkSaleLn (char *line, PRODUCTCAT productCat, CLIENTCAT clientCat) {
	int i, lnOk, quant, month, filial;
	double price;
	char *token;

	lnOk = 1;
	token = strtok(line, " ");

	for (i = 0; lnOk && token != NULL; i++){
		switch(i) {
<<<<<<< HEAD:datacheck.c
			case 0: lnOk = lookUpCatalog(productCat, token);
=======
			case 0: lnOk = lookUpProduct(productCat, toProduct(token));
>>>>>>> master:src/datacheck.c
						break;
			case 1: lnOk = ((price = atof(token)) >= 0 && price <= 999.99);
						break;
			case 2: lnOk = ((quant = atoi(token)) >= 1 && quant <= 200);
						break;
			case 3: lnOk = !strcmp(token, "P") || !strcmp(token, "N");
						break;
<<<<<<< HEAD:datacheck.c
			case 4: lnOk = lookUpCatalog(clientCat, token);
=======
			case 4: lnOk = lookUpClient(clientCat, toClient(token));
>>>>>>> master:src/datacheck.c
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
    CLIENTCAT clients;
	PRODUCTCAT products;
	CLIENT c;
	PRODUCT p;

    clients = initClientCat();
    products = initProductCat();

    while (fgets(line, BUFF_SIZE, file)) {
        token = strtok(line, " \n\r");

        for (i = 0; token != NULL; i++){
    		switch(i) {
<<<<<<< HEAD:datacheck.c
    			case 0: if (!lookUpCatalog(products, token)) {
                            totalProducts++;
                            insertCatalog(products, token);
=======
    			case 0: p = toProduct(token);
                        if (!lookUpProduct(products, p)) {
                            totalProducts++;
                            insertProduct(products, p);
>>>>>>> master:src/datacheck.c
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
<<<<<<< HEAD:datacheck.c
    			case 4: if (!lookUpCatalog(clients, token)) {
                            totalClients++;
                            insertCatalog(clients, token);
=======
    			case 4: c = toClient(token);
						if (!lookUpClient(clients, c)) {
                            totalClients++;
                            insertClient(clients, c);
>>>>>>> master:src/datacheck.c
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
    printf("Unidades Vendidas: %d\n", totalQuant);
    printf("Total Clientes: %d\n", totalClients);
    printf("Total Produtos: %d\n", totalProducts);
}
