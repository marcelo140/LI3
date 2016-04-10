#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "datacheck.h"
#include "sales.h"

#define BUFF_SIZE 35

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

int checkSales (FILE *file, FATGLOBAL fat, PRODUCTCAT products, CLIENTCAT clients, int *sucLn, int *failLn) {
	SALE s = initSale();
	char buf[BUFF_SIZE], *line;
	int suc, total;
	time_t begin, end;
	double time = 0;

	suc = total = 0;

	while(fgets(buf, BUFF_SIZE, file)) {
		line = strtok (buf, "\n\r");
		begin = clock();
		s = readSale(s, line);
		end = clock();
		total++;
		
		if (isSale(s, products, clients)) {
			addFat(fat, s);	
			freeSale(s, M_CONTENT);
		 	suc++;
		}
		time += (double) (end - begin) / CLOCKS_PER_SEC;
	}

	printf("tempo %fs\n", time);

	free(s);

	*sucLn = suc;
	*failLn = total - suc;

	return 0;
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
