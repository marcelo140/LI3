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
		freeClient(client);
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
		freeProduct(product);
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
		 	suc++;
		}
	
		freeSale(s, M_CONTENT);
		time += (double) (end - begin) / CLOCKS_PER_SEC;
	}

	printf("tempo %fs\n", time);

	free(s);
	*sucLn = suc;
	*failLn = total - suc;

	return 0;
}
