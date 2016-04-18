#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "interpreter.h"
#include "clients.h"
#include "products.h"
#include "queries.h"

#define TYPE_CATSET 0
#define TYPE_PSET 1

#define UPPER(a) (('a' <= a && a <= 'z') ? (a - 'a' + 'A') : a)
#define PRODUCT_SIZE 6
#define LINES_NUM 20
#define STR_SIZE 128
#define BUFF_SIZE 255

struct printset{
	char* header;
	char** list;
	int size;
	int capacity;
}; 
	
static void presentList(PRINTSET ps);
static void presentCatalogSet (CATSET* cs,int branches, int page, int total, int* cont);
static void presentProductSet (PRODUCTSET ps, int page, int total, int* cont);
static int askBranch(); 
static PRODUCT askProduct();
static int askMonth();

/*Devolve numero de comandos executados */
int interpreter(BRANCHSALES* bs, FATGLOBAL fat, PRODUCTCAT pcat, CLIENTCAT ccat) {
	PRODUCT p;
	PRINTSET ps;
	char answ[BUFF_SIZE];
	int qnum, option, n;

	system("clear");
	putchar('\n');
	printf("           _____            __      __            _               \n");
	printf("          / ____|           \\ \\    / /           | |              \n");
	printf("         | |  __  ___ _ __ __\\ \\  / /__ _ __   __| | __ _ ___     \n");
	printf("         | | |_ |/ _ \\ '__/ _ \\ \\/ / _ \\ '_ \\ / _` |/ _` / __|    \n");
	printf("         | |__| |  __/ | |  __/\\  /  __/ | | | (_| | (_| \\__ \\    \n");
	printf("          \\_____|\\___|_|  \\___| \\/ \\___|_| |_|\\__,_|\\__,_|___/  \n\n");
	printf("  :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
	printf("                                                         \n");
	printf("      1 • Leitura                                         \n");
	printf("      2 • Listar Produtos                                 \n");
	printf("      3 • Receita do Produto por Mês                      \n");
	printf("      4 • Produtos Não Comprados                          \n");
	printf("      5 • Gastos de um Cliente                            \n");
	printf("      6 • Vendas de Meses                                 \n");
	printf("      7 • Clientes Que Compraram em Todas as Filiais      \n");
	printf("      8 • Clientes Que Compraram Produto em Filial        \n");
	printf("      9 • Produto Mais Compardo por Cliente em Mês        \n");
	printf("     10 • Produtos Mais Vendidos em Todo o Ano            \n");
	printf("     11 • Três Produtos em que o Cliente mais gastou      \n");
	printf("     12 • Clientes sem Compras & Produtos não Vendidos    \n");
	printf("                                                         \n");
	printf("      q • Sair                                            \n");
	printf("                                                         \n");
	printf("  :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");

	printf("\n  Escolha uma opção : ");

	fgets(answ, BUFF_SIZE, stdin);

	if (answ[0] == 'q') return 0;

	qnum = atoi(answ);

	switch(qnum) {
		case 1 :
				 break;
		case 2 : printf("  Letra: ");
				 fgets(answ, BUFF_SIZE, stdin);
				 ps = query2(pcat, answ[0]);
				 if (ps) presentList(ps);
				 freePrintSet(ps); 
				 break;
		case 3 : p = askProduct(); 
				 if (!p) break;
				 option = askMonth();
				 if (option == -1) break; 
				 ps = query3(fat, p, option);
				 if (ps) presentList(ps);
			 	 break;
		case 4 : ps = query4(fat);
				 if (ps) presentList(ps);
				 freePrintSet(ps);
				 break;
		case 5 : option = askBranch();
				 printf("  Cliente: ");
				 fgets(answ, BUFF_SIZE, stdin);
				 answ[CLIENT_LENGTH-1] = '\0';
				 ps = query5(bs[option], toClient(answ));
				 presentList(ps); 
		 		 break;
		case 6 : 
		  		 break;
		case 7 :
				 break;
		case 8 : option = askBranch();
				 printf("Produto: ");
				 fgets(answ, BUFF_SIZE, stdin);
				 answ[PRODUCT_LENGTH-1] = '\0';
				 p = toProduct(answ);
				 ps = query8(bs[option], p);
				 if (ps) presentList(ps);
				 freeProduct(p);
			     if (ps) freePrintSet(ps); 
		 		 break;
		case 9 :
		 		 break;
		case 10 : option = askBranch();
				  printf("  Quantos: ");
				  fgets(answ, BUFF_SIZE, stdin);
				  n = atoi(answ);
				  ps = query10(bs[option], n);
				  presentList(ps);
				  freePrintSet(ps);
		 		  break;
		case 11 :
		 		  break;
		case 12 :
				  break; 
		default : return interpreter(bs, fat, pcat, ccat);
	}
	
	return interpreter(bs, fat, pcat, ccat) + 1;
}

PRINTSET initPrintSet(int n) {
	PRINTSET new = malloc(sizeof(*new));

	new->capacity = n;
	new->size = 0;
	new->list = calloc(n, sizeof(char*));
	new->header = NULL;

	return new;
}

PRINTSET setPrintHeader(PRINTSET ps, char *header) {
	ps->header = malloc(STR_SIZE * sizeof(char));
	strncpy(ps->header, header, STR_SIZE);	

	return ps;
}

PRINTSET addToPrintSet(PRINTSET ps, char* str) {
	
	if (ps->size >= ps->capacity) {
		ps->capacity *= 2;
		ps->list = realloc(ps->list, ps->capacity * sizeof(char*));
	}

	ps->list[ps->size] = calloc(STR_SIZE, sizeof(char)); 
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

	for(i=0; i < ps->capacity; i++)
		free(ps->list[i]);
	free(ps->list);
	free(ps->header);
	free(ps);
}


static void presentList(PRINTSET ps) {
	char nav, onav, option[BUFF_SIZE], **print;
	int i, cpage = 1, totalPages, o, total = ps->size;

	nav = '\n';
	onav = '\n';

	totalPages = (total / LINES_NUM) + (total % LINES_NUM != 0);

	while (cpage <= totalPages) {
		
		system("clear");
		putchar('\n');

		printf("::::::::::::::::::::: PÁGINA %d de %d :::::::::::::::::::::\n\n", cpage, totalPages);

		if (ps->header) printf("  %s\n", ps->header);

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

/*
static void presentProductSet (PRODUCTSET ps, int page, int total, int* cont) {
	int i;
	char str[10];
	PRODUCT p;
	*cont = (page-1) * LINES_NUM;

	for (i=0; i < LINES_NUM && *cont < total; *cont += 1, i++) {

		p = getProductByPos(ps, (page -1) * LINES_NUM + i);
		fromProduct(p, str);

		printf("\t\t%s\n", str);
		
		freeProduct(p);
	}
}

static void presentCatalogSet (CATSET* cs,int branches, int page, int total, int* cont) {
	int i, j;
	char *str;
	*cont = (page-1) * LINES_NUM;

	if (branches > 1) {
		for (i=0; i < branches; i++)
			printf(" FILIAL %d\t", i+1);
		putchar('\n');
		putchar('\n');
	}

	for (i=0; i < LINES_NUM && *cont < total; *cont += 1, i++) {

		for (j=0; j < branches; j++) {
			if (branches == 1) printf("\t\t");
			str = getKeyPos(cs[j], (page -1) * LINES_NUM + i);
			if (str) printf(" %s\t\t", str);
			else printf("\t\t");
		}
		putchar('\n');
			free(str); 
	}
}


static void query2(PRODUCTCAT pcat) {

	char answ;
	PRINTSET ps;
	PRODUCTSET prodS = initProductSet(100);

	do {
		printf("\nLetra: ");
		answ = getchar();
		if (answ == 'q') return;
	} while (answ < 'A' && answ > 'z');

	prodS = fillProductSet(pcat, prodS, answ);

	ps.setNums = 1;
	ps.type = TYPE_PSET;
	ps.size = getProductSetSize(prodS);
	ps.set = prodS;

	presentList(ps);

	freeProductSet(prodS);
}

static void query3(FATGLOBAL fat) {

	char answ, buff[BUFF_SIZE];
	int i, month, mode, quantN, quantP;
	double billedN, billedP;
	FATDATA fd;

 	putchar('\n');

	do {
		printf("Mês (1-12): ");
		fgets(buff, BUFF_SIZE, stdin);
		month = atoi(buff);
		if (buff[0] == 'q') return;
	} while (month <= 0 || month > 12);


	printf("Produto: ");
	fgets(buff, BUFF_SIZE, stdin);
	if (buff[0] == 'q') return;
	buff[PRODUCT_SIZE] = '\0';

	do {
		printf("\nApresentar por: \n");
		printf(" 1• Filial\n");
		printf(" 2• Total\n");
		printf(" : ");
		answ = getchar();
		if (answ == 'q') return;
	} while (answ != '1' && answ != '2');

	mode = (answ == '1') ? BRANCHES : TOTAL;

	fd = monthRevenue(fat, buff, month-1, mode);

	putchar('\n');
	for (i=0; i < mode; i++) {
		getBilledFat (fd, i, &billedN, &billedP);
		getQuantFat  (fd, i, &quantN, &quantP);
		if (mode == BRANCHES) printf(" ::::::::::::::::::::: FILIAL %d :::::::::::::::::::::\n", i+1);
		printf("Nº Vendas N: %d\t\tTotal Faturado N: %.2f\n", quantN, billedN);
		printf("Nº Vendas P: %d\t\tTotal Faturado P: %.2f\n", quantP, billedP);
		putchar('\n');
	}

	while( getchar() != '\n');
	getchar();
}

 Produtos não Vendidos 
static void query4(FATGLOBAL fat) {

	char answ;
	int mode, max, tmp, i;
	CATSET *cs;
	PRINTSET ps;

	do {
		printf("\nApresentar por: \n");
		printf(" 1• Filial\n");
		printf(" 2• Total\n");
		printf(": ");
		answ = getchar();
	} while (answ != '1' && answ != '2');

	mode = (answ == '1') ? BRANCHES : TOTAL;

	cs = notSold(fat, mode);
	printf("%d\n", mode);

	max = getCatalogSetSize(cs[0]);

	if (mode != 1)
		for (i=0; i < BRANCHES; i++) {
			tmp = getCatalogSetSize(cs[i]);
			max = (tmp > max) ? tmp : max;
		}

	ps.set = cs;
	ps.size = max;
	ps.type = TYPE_CATSET;
	ps.setNums = mode;

	presentList(ps);
}

static void query6(FATGLOBAL fat) {

	char buff[BUFF_SIZE];
	int inicio, fim, quant;
	double billed;

	printf("  Intervalo de Meses:\n");

	do {
		printf("  Início: ");
		fgets(buff, BUFF_SIZE, stdin);
		if (buff[0] == 'q') return;
		inicio = atoi (buff);
	} while (inicio < 1 || inicio > 12);

	do {
		printf("  Fim: ");
		fgets(buff, BUFF_SIZE, stdin);
		if (buff[0] == 'q') return;
		fim = atoi(buff);
	} while (fim < inicio || fim > 12);

	monthRange(fat, inicio-1, fim-1, &quant, &billed);

	putchar('\n');

	printf(" :::::::::::::::::::::::::\n");
	printf(" Quantidade vendida:  %d\n", quant);
	printf(" Total Faturado:      %f\n", billed);
	putchar('\n');

	getchar();
}
*/
