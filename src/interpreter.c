#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "interpreter.h"
#include "clients.h"
#include "products.h"

#define TYPE_CATSET 0

#define LINES_NUM 20
#define BUFF_SIZE 255

typedef struct {
	void * set;
	int setNums;
	int size;
	int type;
} PRINTSET;

static void query1();
static void query2();
static void query3();
static void query4();
static void query5();
static void query6();
static void query7();
static void query8();
static void query9();
static void query10();
static void query11();
static void query12();
static void presentCatSet (CATSET* cs,int branches, int page, int total, int* cont);

void presentList(PRINTSET ps) {
	char nav, onav, option[BUFF_SIZE];
	int cpage = 1, totalPages, cont=0, o, total = ps.size;

	nav = '\n';
	onav = '\n';

	totalPages = (total / LINES_NUM) + (total % LINES_NUM != 0);

	while (cpage <= totalPages) {

		system("clear");
		putchar('\n');

		printf(":::::::::::: PÁGINA %d de %d ::::::::::::\n\n", cpage, totalPages);
		presentCatSet(ps.set, ps.setNums, cpage, total, &cont);
		printf(":::::::::::::::::::::::::::::::::::::::::\n");
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
			printf("\t<enter> Utiliza o último comando.\n");
			getchar();
		}
		else if (nav == 'q')
			break;
	}
}

static void presentCatSet (CATSET* cs,int branches, int page, int total, int* cont) {
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
		}
		putchar('\n');
		/*	free(str); */
	}
}

/*Devolve numero de comandos executados */
int interpreter(FATGLOBAL fat, PRODUCTCAT pcat, CLIENTCAT ccat) {

	char answ[BUFF_SIZE];
	int qnum;

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

	printf("\n Escolha uma opção : ");

	fgets(answ, BUFF_SIZE, stdin);

	if (answ[0] == 'q') return 0;

	qnum = atoi(answ);

	switch(qnum) {
		case 1 : query1();
				 break;
		case 2 : query2(pcat);
				 break;
		case 3 : query3(fat);
			 	 break;
		case 4 : query4(fat);
				 break;
		case 5 : query5();
		 		 break;
		case 6 : query6();
		  		 break;
		case 7 : query7();
				 break;
		case 8 : query8();
		 		 break;
		case 9 : query9();
		 		 break;
		case 10 : query10();
		 		  break;
		case 11 : query11();
		 		  break;
		case 12 : query12();
				  break;
		default : return interpreter(fat, pcat, ccat);
	}

	getchar();

	return interpreter(fat, pcat, ccat) + 1;
}

static void query1() {

}

static void query2() {
/*
	char answ;
	CATSET *cs;

	do {
		printf("\nLetra: ");
		answ = getchar();
	} while (answ < 'A' && answ > '2');

	mode = (answ == '1') ? BRANCHES : TOTAL;

	cs = notSold(fat, mode);

	present(cs[0]);
*/
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
	} while (month <= 0 || month > 12);

	putchar('\n');

	printf("Produto: ");
	fgets(buff, BUFF_SIZE, stdin);

	do {
		printf("\nApresentar por: \n");
		printf(" 1• Filial\n");
		printf(" 2• Total\n");
		printf(" : ");
		answ = getchar();
	} while (answ != '1' && answ != '2');
	
	mode = (answ == '1') ? BRANCHES : TOTAL;
	
	fd = monthRevenue(fat, buff, month-1, mode);

	putchar('\n');
	for (i=0; i < mode; i++) {
		getBilledFat (fd, i, &billedN, &billedP);
		getQuantFat  (fd, i, &quantN, &quantP);
		if (mode == BRANCHES) printf("\tFILIAL %d\n", i+1);
		printf("Nº Vendas N: %d\tTotal Faturado N: %f\n", quantN, billedN);
		printf("Nº Vendas P: %d\tTotal Faturado P: %f\n", quantP, billedP);
	}
}

/* Produtos não Vendidos */
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

	max = getCatSetSize(cs[0]);

	if (mode != 1) 
		for (i=0; i < BRANCHES; i++) {
			tmp = getCatSetSize(cs[i]);
			max = (tmp > max) ? tmp : max;
		}
	 
	ps.set = cs;
	ps.size = max; 
	ps.type = TYPE_CATSET;
	ps.setNums = mode;

	presentList(ps);	
}

static void query5() {

}

static void query6() {

}

static void query7() {

}

static void query8() {

}

static void query9() {

}

static void query10() {

}

static void query11() {

}

static void query12() {

}
