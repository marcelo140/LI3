#include <stdio.h>
#include <time.h>

#include "interpreter.h"
#include "datacheck.h"
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

	double time;
	time_t end, begin;

	clients = fopen(CLIENTS_PATH, "r");
	products = fopen(PRODUCTS_PATH, "r");
	sales = fopen(SALES_PATH, "r");

	if (!(clients && products && sales)){
		fprintf(stderr,"Ficheiros de entrada não encontrados");
		return -1;
	}	
	clientCat = initClientCat();
	productCat = initProductCat();

	begin = clock();
	clientCat = writeCCat(clients, clientCat, &suc);
	end = clock();
	time = (double) (end - begin) / CLOCKS_PER_SEC;
	printf("Clientes analisados: %d (%fs)\n", suc, time);


	putchar('\n');

	begin = clock();
	productCat = writePCat(products, productCat, &suc);
	end = clock();
	time = (double) (end - begin) / CLOCKS_PER_SEC;
	printf("Produtos analisados: %d (%fs)\n", suc, time);

	putchar('\n');
	
	begin = clock();
	fat = initFat(productCat);
	end = clock();
	time = (double) (end - begin) / CLOCKS_PER_SEC;
	printf("FillFat (%fs)\n", time);

	putchar('\n');


	begin = clock();
	checkSales(sales, fat, productCat, clientCat, &suc, &fail);
	end = clock();
	time = (double) (end - begin) / CLOCKS_PER_SEC;
	printf("Vendas analisadas: %d (%fs)\n", suc+fail, time);
	printf("Vendas corretas: %d\n", suc);
	printf("Vendas incorretas: %d\n", fail);

	putchar('\n');

	begin = clock();
	notSold(fat, TOTAL);
	end = clock();
	time = (double) (end - begin) / CLOCKS_PER_SEC;
	printf("tempo: %fs\n", time);

	freeClientCat(clientCat);
	freeProductCat(productCat);
	freeFat(fat);

	fclose(clients);
	fclose(products);
	fclose(sales);

	return 0;
}
