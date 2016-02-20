#include <stdio.h>
#include "datacheck.h"

#define SALES_PATH "Vendas_5M.txt"
#define CLIENTS_PATH "Clientes.txt"
#define PRODUCTS_PATH "Produtos.txt"

typedef char bool;

int main() {
	FILE *clients, *products, *sales;
	int suc, fail;

	clients = fopen(CLIENTS_PATH, "r");
	products = fopen(PRODUCTS_PATH, "r");
	sales = fopen(SALES_PATH, "r");

	checkFile(products, M_PRODUCTS, &suc, &fail);
	printf("Produtos analisados: %d\n", suc+fail);
	printf("Produtos corretos: %d\n", suc);
	printf("Produtos incorretos: %d\n", fail);

	putchar('\n');

	checkFile(clients, M_CLIENTS, &suc, &fail);
	printf("Clientes analisados: %d\n", suc+fail);
	printf("Clientes corretos: %d\n", suc);
	printf("Clientes incorretos: %d\n", fail);

	putchar('\n');

	checkFile(sales, M_SALES, &suc, &fail);
	printf("Vendas analisadas: %d\n", suc+fail);
	printf("Vendas corretas: %d\n", suc);
	printf("Vendas incorretas: %d\n", fail);

	fclose(clients);
	fclose(products);
	fclose(sales);

	return 0;
}
