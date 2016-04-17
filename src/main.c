#include <stdio.h>
#include <time.h>

#include "interpreter.h"
#include "dataloader.h"
#include "clients.h"
#include "products.h"

#define SALES_PATH "Vendas_1M.txt"
#define CLIENTS_PATH "Clientes.txt"
#define PRODUCTS_PATH "Produtos.txt"

int main() {
	FILE *clients, *products, *sales;
	FATGLOBAL fat;
	BRANCHSALES branchSales[3];
	CLIENTCAT clientCat;
	PRODUCTCAT productCat;
	int success, failed;

	clients = fopen(CLIENTS_PATH, "r");
	products = fopen(PRODUCTS_PATH, "r");
	sales = fopen(SALES_PATH, "r");

	if (!(clients && products && sales)){
		fprintf(stderr,"Ficheiros de entrada não encontrados");
		return -1;
	}

	clientCat = initClientCat();
	productCat = initProductCat();


	success = loadClients(clients, clientCat);
	printf("Clientes analisados: %d\n", success);


	putchar('\n');

	success = loadProducts(products, productCat);
	printf("Produtos analisados: %d\n", success);

	putchar('\n');

	fat = initFat(productCat);

	for( failed = 0; failed < 3; failed++)
		branchSales[failed] = initBranchSales(clientCat);

	putchar('\n');

	success = loadSales(sales, fat, branchSales, productCat, clientCat, &failed);
	printf("Vendas analisadas: %d\n", success+failed);
	printf("Vendas corretas: %d\n", success);
	printf("Vendas incorretas: %d\n", failed);

	putchar('\n');

	interpreter(branchSales, fat, productCat, clientCat);

	return 0;
}
