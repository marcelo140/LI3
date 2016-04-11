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
static void presentCatSet (CATSET cs, int page, int total, int *cont);

void present(PRINTSET ps) {
	char nav, onav, option[BUFF_SIZE];
	int cpage = 1, totalPages, cont=0, o, total = ps.size;

	nav = '\n';
	onav = '\n';

	totalPages = (total / LINES_NUM) + (total % LINES_NUM != 0);

	putchar('\n');
	while (cpage <= totalPages) {

		system("clear");
		putchar('\n');

		printf(":::::::::::: PÁGINA %d de %d ::::::::::::\n\n", cpage, totalPages);
		presentCatSet(ps.set, cpage, total, &cont);
		printf("\nb: Anterior\tn: Seguinte\th: Ajuda\ng: Ir para página\tq: Sair\n\t>> ");

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
			if (cpage + o > totalPages) cpage = totalPages;
			else if (cpage - o <= 0) cpage = 0;
			else cpage = (cpage == 1) ? cpage : cpage-o;
		} else if (nav == 'g') {
			if (o > totalPages)	o = totalPages;
			else if (o <= 0) o = 1;
			cpage = o;			
		}else if (nav == 'h') {
			printf("b<num>  Retrocede <num> páginas.\n"); 
			printf("n<num>  Avança <num> páginas.\n"); 
			printf("g<num>  Salta para a página número <num>.\n");
			printf("<enter> Utiliza o último comando.\n");
			getchar();
		}
		else if (nav == 'q')
			break;
	}
}

static void presentCatSet (CATSET cs, int page, int total, int* cont) {
	int i;
	char *str;
	*cont = (page-1) * LINES_NUM;

	for (i=0; i < LINES_NUM && *cont < total; *cont += 1, i++) {
		str = getKeyPos(cs, (page -1) * LINES_NUM + i);

		printf("\t\t%s\n", str);

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
		case 3 : query3();
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

	return 1;
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

static void query3() {

}

/* Produtos não Vendidos */
static void query4(FATGLOBAL fat) {

	char answ;
	int mode;
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

	ps.set = cs[0];
	ps.size = getCatSetSize(cs[0]);
	ps.type = TYPE_CATSET;

	present(ps);
	
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
