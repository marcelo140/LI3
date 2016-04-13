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
	BRANCHSALES branchSales = initBranchSales();
	CLIENTCAT clientCat;
	PRODUCTCAT productCat;
	int success, failed;

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
	success = loadClients(clients, clientCat);
	end = clock();
	time = (double) (end - begin) / CLOCKS_PER_SEC;
	printf("Clientes analisados: %d (%fs)\n", success, time);


	putchar('\n');

	begin = clock();
	success = loadProducts(products, productCat);
	end = clock();
	time = (double) (end - begin) / CLOCKS_PER_SEC;
	printf("Produtos analisados: %d (%fs)\n", success, time);

	putchar('\n');

	begin = clock();
	fat = initFat(productCat);
	end = clock();
	time = (double) (end - begin) / CLOCKS_PER_SEC;
	printf("FillFat (%fs)\n", time);

	putchar('\n');

	begin = clock();
	success = loadSales(sales, fat, branchSales, productCat, clientCat, &failed);
	end = clock();
	time = (double) (end - begin) / CLOCKS_PER_SEC;
	printf("Vendas analisadas: %d (%fs)\n", success+failed, time);
	printf("Vendas corretas: %d\n", success);
	printf("Vendas incorretas: %d\n", failed);

	putchar('\n');

	getchar();

	interpreter(fat, productCat, clientCat);

	freeClientCat(clientCat);
	freeProductCat(productCat);
	freeFat(fat);
/*	freeBranchSale(branchSales); */

	fclose(clients);
	fclose(products);
	fclose(sales);

	return 0;
}
