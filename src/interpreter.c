#include <stdlib.h>
#include <stdio.h>
#include <time.h> /* <<---------- TIME.H */
#include <string.h>
#include "dataloader.h"
#include "interpreter.h"
#include "clients.h"
#include "products.h"
#include "queries.h"

#define UPPER(a) (('a' <= a && a <= 'z') ? (a - 'a' + 'A') : a)
#define PRODUCT_SIZE s
#define LINES_NUM 20
#define STR_SIZE 128
#define BUFF_SIZE 255

#define SALES_PATH "Vendas_1M.txt"
#define CLIENTS_PATH "Clientes.txt"
#define PRODUCTS_PATH "Produtos.txt"

static void    printMainMenu();
static void    printLogo();

int interpreter(BRANCHSALES* bs, FATGLOBAL fat, PRODUCTCAT pcat) {
	char answ[BUFF_SIZE];
	int qnum;


	printMainMenu();
	printf("  Escolha uma opção : ");

	fgets(answ, BUFF_SIZE, stdin);

	if (answ[0] == 'q') return KILL;

	printLogo();

	qnum = atoi(answ);

	switch(qnum) {
		case 1 : return LOAD;
		case 2 : query2(pcat);
				 break;
		case 3 : query3(fat, pcat);
			 	 break;
		case 4 : query4(fat);
				 break;
		case 5 : query5(bs);
		 		 break;
		case 6 : query6(fat);
		  		 break;
		case 7 :
				 break;
		case 8 : query8(bs);
		 		 break;
		case 9 :
		 		 break;
		case 10 : query10(bs);
		 		  break;
		case 11 :
		 		  break;
		case 12 :
				  break;
	}

	return CONT;
}


void loader(BRANCHSALES* bs, FATGLOBAL fat, PRODUCTCAT pcat, CLIENTCAT ccat ) {
	int i, success, failed;
	char clientsPath[BUFF_SIZE], productsPath[BUFF_SIZE], salesPath[BUFF_SIZE];
	FILE *clients, *products, *sales;

	time_t inicio, fim;

	putchar('\n');

	while(1) {
		printf("Ficheiro de clientes: ");
		clientsPath[0] = getchar();
		if (clientsPath[0] == '\n') strcpy(clientsPath, CLIENTS_PATH);
		else fgets(clientsPath+1, BUFF_SIZE-1, stdin);
		strtok(clientsPath, "\n\r");
		clients = fopen(clientsPath, "r");
		if (clients) break;
		printf("Ficheiro %s inválido ou inexistente!\n", clientsPath);
	}

	while(1) {
		printf("Ficheiro de produtos: ");
		productsPath[0] = getchar();
		if (productsPath[0] == '\n') strcpy(productsPath, PRODUCTS_PATH);
		else fgets(productsPath+1, BUFF_SIZE-1, stdin);
		strtok(productsPath, "\n\r");
		products = fopen(productsPath, "r");
		if (products) break;
		printf("Ficheiro %s inválido ou inexistente!\n", productsPath);
	}

	while(1) {
		printf("Ficheiro de vendas: ");
		salesPath[0] = getchar();
		if (salesPath[0] == '\n') strcpy(salesPath, SALES_PATH);
		else fgets(salesPath+1, BUFF_SIZE-1, stdin);
		strtok(salesPath, "\n\r");
		sales = fopen(salesPath, "r");
		if (sales) break;
		printf("Ficheiro %s inválido ou inexistente!\n", salesPath);
	}

	putchar('\n');

	inicio = time(NULL);
	printf("A carregar clientes de %s... ", clientsPath);
	fflush(stdout);
	success = loadClients(clients, ccat);
	printf("\nClientes carregados: %d\n", success);

	putchar('\n');

	printf("A carregar produtos de %s... ", productsPath);
	fflush(stdout);
	success = loadProducts(products, pcat);
	printf("\nProdutos carregados: %d\n", success);

	putchar('\n');

	printf("A carregar vendas de %s... ", salesPath);
	fflush(stdout);
	for(i=0; i < 3; i++)
		bs[i] = fillBranchSales(bs[i], ccat, pcat);
	fat = fillFat(fat, pcat);
	success = loadSales(sales, fat, bs, pcat, ccat, &failed);
	printf("\nVendas analisadas: %d\n", success+failed);
	printf("Vendas corretas: %d\n", success);
	printf("Vendas incorretas: %d\n", failed);
	fim = time(NULL);

	printf("Tudo carregado em %f segundos.\n", difftime(fim, inicio));
	printf("Pressione qualquer tecla para continuar. ");
	getchar();

	fclose(products);
	fclose(clients);
	fclose(sales);
}


static void printMainMenu() {

	printLogo();

	printf("      1 • Leitura de Dados                                \n");
	printf("      2 • Listar Produtos                                 \n");
	printf("      3 • Receita do Produto por Mês                      \n");
	printf("      4 • Produtos Não Comprados                          \n");
	printf("      5 • Gastos de um Cliente                            \n");
	printf("      6 • Vendas no Intervalo de Meses                    \n");
	printf("      7 • Clientes Que Compraram em Todas as Filiais      \n");
	printf("      8 • Clientes Que Compraram Produto em Filial        \n");
	printf("      9 • Produto Mais Comprado por Cliente em Mês        \n");
	printf("     10 • Produtos Mais Vendidos em Todo o Ano            \n");
	printf("     11 • Três Produtos em que o Cliente mais gastou      \n");
	printf("     12 • Clientes sem Compras & Produtos não Vendidos    \n");
	printf("                                                         \n");
	printf("      q • Sair                                            \n");
	printf("                                                         \n");
	printf("  :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n\n");
}

static void printLogo() {

	system("clear");
	putchar('\n');
	printf("           _____            __      __            _               \n");
	printf("          / ____|           \\ \\    / /           | |              \n");
	printf("         | |  __  ___ _ __ __\\ \\  / /__ _ __   __| | __ _ ___     \n");
	printf("         | | |_ |/ _ \\ '__/ _ \\ \\/ / _ \\ '_ \\ / _` |/ _` / __|    \n");
	printf("         | |__| |  __/ | |  __/\\  /  __/ | | | (_| | (_| \\__ \\    \n");
	printf("          \\_____|\\___|_|  \\___| \\/ \\___|_| |_|\\__,_|\\__,_|___/  \n\n");
	printf("  :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n\n");

}
