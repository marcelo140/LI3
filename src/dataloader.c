#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "dataloader.h"
#include "sales.h"

#define BUFF_SIZE 35

int loadClients(FILE *file, CLIENTCAT cat) {

	char buf[BUFF_SIZE], *line;
	CLIENT client;
	int success;

	client = newClient();
	success = 0;

	while(fgets(buf, BUFF_SIZE, file)) {
		line = strtok (buf, "\n\r");
		client = writeClient(client, line);

		cat = insertClient(cat, client);
		success++;
	}

	freeClient(client);

	return success;
}

int loadProducts(FILE *file, PRODUCTCAT cat) {

	char buf[BUFF_SIZE], *line;
	PRODUCT product;
	int success;

	product = newProduct();
	success = 0;

	while(fgets(buf, BUFF_SIZE, file)) {
		line = strtok (buf, "\n\r");
		product = writeProduct(product, line);

		cat = insertProduct(cat, product);
		success++;
	}

	freeProduct(product);

	return success;
}

int loadSales(FILE *file, FATGLOBAL fat, BRANCHSALES* bs, PRODUCTCAT products, 
              CLIENTCAT clients, int *failed) {

	char buffer[BUFF_SIZE], *line;
	PRODUCT prod;
	CLIENT c;
	SALE s;
	int success, total;

	prod = newProduct();
	c = newClient();
	s = initSale();
	success = total = 0;

	while(fgets(buffer, BUFF_SIZE, file)) {
		line = strtok (buffer, "\n\r");
		s = readSale(s, prod, c, line);
		total++;
		
		if (isSale(s, products, clients)) {
			addFat(fat, s);	
			addSaleToBranch(bs[getBranch(s)], s);
		 	success++;
		}
	}

	freeSale(s);
	freeProduct(prod);
	freeClient(c);

	*failed = total - success;

	return success;
}
