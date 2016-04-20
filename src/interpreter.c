#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dataloader.h"
#include "interpreter.h"
#include "clients.h"
#include "products.h"
#include "queries.h"

#define UPPER(a) (('a' <= a && a <= 'z') ? (a - 'a' + 'A') : a)
#define PRODUCT_SIZE 6
#define LINES_NUM 20
#define STR_SIZE 128
#define BUFF_SIZE 255

#define SALES_PATH "Vendas_1M.txt"
#define CLIENTS_PATH "Clientes.txt"
#define PRODUCTS_PATH "Produtos.txt"

struct printset{
	char header[STR_SIZE];
	char** list;
	int size;
	int capacity;
};

static void    presentList(PRINTSET ps);
static void    printMainMenu();
static void    printLogo();
static int     askBranch();
static PRODUCT askProduct();
static int     askMonth();

int interpreter(BRANCHSALES* bs, FATGLOBAL fat, PRODUCTCAT pcat) {
	PRODUCT p;
	CLIENT client;
	PRINTSET ps = NULL;
	char answ[BUFF_SIZE];
	int qnum, x, y;


	printMainMenu();
	printf("  Escolha uma opção : ");

	fgets(answ, BUFF_SIZE, stdin);

	if (answ[0] == 'q') return KILL;

	printLogo();

	qnum = atoi(answ);

	switch(qnum) {
		case 1 : return LOAD;
		case 2 : printf("  Letra: ");
				 fgets(answ, BUFF_SIZE, stdin);
				 ps = query2(pcat, answ[0]);
				 if (ps) presentList(ps);
				 break;
		case 3 : p = askProduct();
				 if (!p) break;
				 x = askMonth();
				 if (x == -1) break;
				 ps = query3(fat, p, x);
				 freeProduct(p);
				 getchar();
				 if (ps) presentList(ps);
			 	 break;
		case 4 : ps = query4(fat);
				 if (ps) presentList(ps);
				 break;
		case 5 : x = askBranch();
				 printf("  Cliente: ");
				 fgets(answ, BUFF_SIZE, stdin);
				 answ[CLIENT_LENGTH-1] = '\0';
				 client = toClient(answ);
				 ps = query5(bs[x], client);
				 freeClient(client);
				 presentList(ps);
		 		 break;
		case 6 : printf("Mês inicial: ");
				 fgets(answ, BUFF_SIZE, stdin);
				 x = atoi(answ);
				 printf("Mês final: ");
				 fgets(answ, BUFF_SIZE, stdin);
				 y = atoi(answ);
				 ps = query6(fat, x, y);
				 if (ps) presentList(ps);
		  		 break;
		case 7 : 
				 break;
		case 8 : x = askBranch();
				 printf("Produto: ");
				 fgets(answ, BUFF_SIZE, stdin);
				 answ[PRODUCT_LENGTH-1] = '\0';
				 p = toProduct(answ);
				 ps = query8(bs[x], p);
				 if (ps) presentList(ps);
				 freeProduct(p);
		 		 break;
		case 9 :
		 		 break;
		case 10 : x = askBranch();
				  printf("  Quantos: ");
				  fgets(answ, BUFF_SIZE, stdin);
				  y = atoi(answ);
				  ps = query10(bs[x], y);
				  presentList(ps);
		 		  break;
		case 11 :
		 		  break;
		case 12 :
				  break;
	}

	freePrintSet(ps);
	return CONT;
}

PRINTSET initPrintSet(int n) {
	PRINTSET new = malloc(sizeof(*new));

	new->capacity = n;
	new->size = 0;
	new->list = calloc(n, sizeof(char*));
	new->header[0] = '\0';

	return new;
}

PRINTSET setPrintHeader(PRINTSET ps, char *header) {
	strncpy(ps->header, header, STR_SIZE);

	return ps;
}

PRINTSET addToPrintSet(PRINTSET ps, char* str) {

	if (ps->size >= ps->capacity -1) {
		ps->capacity *= 2;
		ps->list = realloc(ps->list, ps->capacity * sizeof(char*));
		memset(ps->list + ps->size , 0, (ps->capacity - ps->size) * sizeof(char*));
	}

	ps->list[ps->size] = calloc(STR_SIZE+1, sizeof(char));
	strncpy(ps->list[ps->size], str, STR_SIZE);
	ps->size++;

	return ps;
}

char** getPage(PRINTSET ps, int page) {
	int p;

	p = (page-1) * LINES_NUM;

	if (p >= ps->size) return NULL;

	return ps->list + p;
}

void freePrintSet(PRINTSET ps) {
	int i;

	for(i=0; ps->list[i] && i < ps->capacity; i++)
		free(ps->list[i]);
	free(ps->list);
	free(ps);
}

#include <time.h>
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

static void presentList(PRINTSET ps) {
	char nav, onav, option[BUFF_SIZE], **print;
	int i, cpage = 1, totalPages, o, total = ps->size;

	nav = '\n';
	onav = '\n';

	totalPages = (total / LINES_NUM) + (total % LINES_NUM != 0);

	while (cpage <= totalPages) {

		/* system("clear"); */
		putchar('\n');

		printf("::::::::::::::::::::: PÁGINA %d de %d :::::::::::::::::::::\n\n", cpage, totalPages);

		if (ps->header[0] != '\0') printf("  %s\n", ps->header);

		print = getPage(ps, cpage);
		for(i=0; print && print[i] && i < LINES_NUM; i++)
			printf("  %s\n", print[i]);

		printf(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
		printf("\n b: Anterior\tn: Seguinte\th: Ajuda\n g: Ir para página\tq: Sair\n\t>> ");

		onav = nav;
		nav = getchar();

		if (nav == '\n') nav = onav;
		else fgets(option, BUFF_SIZE, stdin);

		if (option[0] != '\n') o = atoi(option);
		else o = 1;

		if (nav == 'n') {
			if (cpage + o > totalPages) cpage = totalPages;
			else if (cpage + o <= 0) cpage = 0;
			else cpage = (cpage == totalPages) ? cpage : cpage+o;
		} else if (nav == 'b') {
			if (cpage - o > totalPages) cpage = totalPages;
			else if (cpage + o <= 0) cpage = 0;
			else cpage = (cpage == 1) ? cpage : cpage-o;
		} else if (nav == 'g') {
			if (o > totalPages)	o = totalPages;
			else if (o <= 0) o = 1;
			cpage = o;
		}else if (nav == 'h') {
			printf("\tb<num>  Retrocede <num> páginas.\n");
			printf("\tn<num>  Avança <num> páginas.\n");
			printf("\tg<num>  Salta para a página número <num>.\n");
			printf("\t<enter> Utiliza o último comando.\n\t");
			getchar();
		}
		else if (nav == 'q')
			break;
	}
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

	/* system("clear"); */
	putchar('\n');
	printf("           _____            __      __            _               \n");
	printf("          / ____|           \\ \\    / /           | |              \n");
	printf("         | |  __  ___ _ __ __\\ \\  / /__ _ __   __| | __ _ ___     \n");
	printf("         | | |_ |/ _ \\ '__/ _ \\ \\/ / _ \\ '_ \\ / _` |/ _` / __|    \n");
	printf("         | |__| |  __/ | |  __/\\  /  __/ | | | (_| | (_| \\__ \\    \n");
	printf("          \\_____|\\___|_|  \\___| \\/ \\___|_| |_|\\__,_|\\__,_|___/  \n\n");
	printf("  :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n\n");

}

static int askBranch() {
	char buff[BUFF_SIZE];
	int r=0;

	printf("\n  ::::::::::::::::::::::::::::::\n");
	printf("\t   1• Filial 1\n");
	printf("\t   2• Filial 2\n");
	printf("\t   3• Filial 3\n");
	printf("  ::::::::::::::::::::::::::::::\n");

	do {
		printf("  Escolha uma filial: ");
		fgets(buff, BUFF_SIZE, stdin);
		r = atoi(buff);
	} while(buff[0] != 'q' && (r < 1 || r > 3));

	return r-1;
}

static PRODUCT askProduct() {
	char buff[BUFF_SIZE];

	while(1) {
		printf("  Produto: ");
		fgets(buff, BUFF_SIZE, stdin);
		if (isProduct(buff)) break;
		if (UPPER(buff[0]) == 'Q') return NULL;
		if (buff[0] != '\n') printf("Produto Inválido!\n");
	}

	return toProduct(buff);
}

static int askMonth() {
	char buff[BUFF_SIZE];
	int mes;

	while(1) {
		printf("  Mês (1-12): ");
		fgets(buff, BUFF_SIZE, stdin);
		mes = atoi(buff);
		if (mes > 0 && mes < 13) break;
		if (UPPER(buff[0]) == 'Q') return -1;
		printf("Um mês deve estar entre 1 e 12.");
	}

	return mes;
}
