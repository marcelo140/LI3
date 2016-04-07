#include <stdio.h>

#include "datacheck.h"
#include "fatglobal.h"
#include "clients.h"
#include "products.h"

#define SALES_PATH "Vendas_1M.txt"
#define CLIENTS_PATH "Clientes.txt"
#define PRODUCTS_PATH "Produtos.txt"

int main() {
	FILE *clients, *products, *sales;
	FATGLOBAL fat;
	CLIENTCAT clientCat;
	PRODUCTCAT productCat;
	int suc, fail;

	clients = fopen(CLIENTS_PATH, "r");
	products = fopen(PRODUCTS_PATH, "r");
	sales = fopen(SALES_PATH, "r");

	if (!(clients && products && sales)){
		fprintf(stderr,"Ficheiros de entrada não encontrados");
		return -1;
	}

	clientCat = initClientCat();
	productCat = initProductCat();

	clientCat = writeCCat(clients, clientCat, &suc);
	printf("Clientes analisados: %d\n", suc);

	putchar('\n');

	productCat = writePCat(products, productCat, &suc);
	printf("Produtos analisados: %d\n", suc);

	putchar('\n');
	fat = fillFat(productCat);

	checkSales(sales, fat, productCat, clientCat, &suc, &fail);
	printf("Vendas analisadas: %d\n", suc+fail);
	printf("Vendas corretas: %d\n", suc);
	printf("Vendas incorretas: %d\n", fail);

	putchar('\n');


	freeClientCat(clientCat);
	freeProductCat(productCat);

	fclose(clients);
	fclose(products);
	fclose(sales);

	return 0;
}
