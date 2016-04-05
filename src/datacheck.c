#include <string.h>
#include <stdlib.h>

#include "datacheck.h"

#define BUFF_SIZE 35

static int checkSaleLn(char *line, PRODUCTCAT product, CLIENTCAT client);

CLIENTCAT writeCCat(FILE *file, CLIENTCAT cat, int *num) {
	int n;
	char buf[BUFF_SIZE], *line;
	CLIENT client;

	n = 0;

	while(fgets(buf, BUFF_SIZE, file)) {
		line = strtok (buf, "\n\r");
		client = toClient(line);

		cat = insertClient(cat, client);
		n++;
	}

	*num = n;

	return cat;
}

PRODUCTCAT writePCat(FILE *file, PRODUCTCAT cat, int *num) {
	int n;
	char buf[BUFF_SIZE], *line;
	PRODUCT product;

	n = 0;

	while(fgets(buf, BUFF_SIZE, file)) {
		line = strtok (buf, "\n\r");
		product = toProduct(line);

		cat = insertProduct(cat, product);
		n++;
	}

	*num = n;

	return cat;
}

int checkSales (FILE *file, PRODUCTCAT products, CLIENTCAT clients, int *sucLn, int *failLn) {
	int checked_line, suc, fail;
	char buf[BUFF_SIZE], *line;

	suc = fail = 0;

	while(fgets(buf, BUFF_SIZE, file)) {
		line = strtok (buf, "\n\r");
		checked_line = checkSaleLn(line, products, clients);

		(checked_line) ? suc++ : fail++;
	}

	*sucLn = suc;
	*failLn = fail;

	return 0;
}

static int checkSaleLn (char *line, PRODUCTCAT productCat, CLIENTCAT clientCat) {
	int i, lnOk, quant, month, branch;
	double price;
	char *token;

	lnOk = 1;
	token = strtok(line, " ");

	for (i = 0; lnOk && token != NULL; i++){
		switch(i) {
			case 0: lnOk = lookUpProduct(productCat, toProduct(token));
						break;
			case 1: lnOk = ((price = atof(token)) >= 0 && price <= 999.99);
						break;
			case 2: lnOk = ((quant = atoi(token)) >= 1 && quant <= 200);
						break;
			case 3: lnOk = !strcmp(token, "P") || !strcmp(token, "N");
						break;
			case 4: lnOk = lookUpClient(clientCat, toClient(token));
						break;
			case 5: lnOk = (month = atoi(token) >= 1 && month <= 12);
						break;
			case 6: lnOk = ((branch = atoi(token)) >= 1 && branch <= 3);
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
    			case 0: p = toProduct(token);
                        if (!lookUpProduct(products, p)) {
                            totalProducts++;
                            insertProduct(products, p);
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
    			case 4: c = toClient(token);
						if (!lookUpClient(clients, c)) {
                            totalClients++;
                            insertClient(clients, c);
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
