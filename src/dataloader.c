#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "dataloader.h"
#include "sales.h"

#define CODE_BUFFER 32
#define SALE_BUFFER 128

int loadClients(FILE *file, CLIENTCAT cat) {

	char buf[CODE_BUFFER], *clientCode;
	CLIENT client;
	int success;

	client = newClient();
	success = 0;

	while(fgets(buf, CODE_BUFFER, file)) {
		clientCode = strtok (buf, "\n\r");
		client = changeClientCode(client, clientCode);

		cat = insertClient(cat, client);
		success++;
	}

	freeClient(client);

	return success;
}

int loadProducts(FILE *file, PRODUCTCAT cat) {

	char buf[CODE_BUFFER], *productCode;
	PRODUCT product;
	int success;

	product = newProduct();
	success = 0;

	while(fgets(buf, CODE_BUFFER, file)) {
		productCode = strtok (buf, "\n\r");
		product = changeProductCode(product, productCode);

		cat = insertProduct(cat, product);
		success++;
	}

	freeProduct(product);

	return success;
}

int loadSales(FILE *file, FATGLOBAL fat, BRANCHSALES* bs, PRODUCTCAT products, 
              CLIENTCAT clients, int *failed) {

	char buffer[SALE_BUFFER], *line;
	PRODUCT prod;
	CLIENT client;
	SALE s;
	int success, total;

	prod = newProduct();
	client = newClient();
	s = initSale();
	success = total = 0;

	while(fgets(buffer, SALE_BUFFER, file)) {
		line = strtok (buffer, "\n\r");
		s = readSale(s, prod, client, line);
		total++;
		
		if (isSale(s, products, clients)) {
			addSaleToFat(fat, s);	
			addSaleToBranch(bs[getBranch(s)], s);
		 	success++;
		}
	}

	freeSale(s);
	freeProduct(prod);
	freeClient(client);

	*failed = total - success;

	return success;
}
