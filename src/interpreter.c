#include <stdlib.h>
#include <stdio.h>
#include "interpreter.h"
#include "clients.h"
#include "products.h"

#define LINES_NUM 20
#define BUFF_SIZE 255

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

void presentOld(PRODUCTSET ps) {
	char *str, nav, onav;
	int i, cpage = 1, totalPages, cont=0, total = getPSetSize(ps) ;
	PRODUCT p;

	nav = '\n';
	onav = '\n';

	totalPages = (total / LINES_NUM) + (total % LINES_NUM != 0);

	while (cpage <= totalPages) {

		putchar('\n');
		system("clear");

		printf(":::::::::::: PÁGINA %d de %d ::::::::::::\n", cpage, totalPages);

		for (i=0; i < LINES_NUM && cont < total; cont++, i++) {
			p = getPSetData(ps, cpage * LINES_NUM + i);

			str = fromProduct(p);

			printf("\t\t%s\n", str);

 	/*		freeProduct(p); */
			free(str);
		}
		printf("\nb: Anterior\tn: Seguinte\tq: Sair\th: Ajuda\n\t>> ");

		onav = nav;
		nav = getchar();
		if (nav == '\n') nav = onav;
		else while(getchar() != '\n');

		if (nav == 'n')
			cpage = (cpage == totalPages) ? cpage : cpage+1;
		else if (nav == 'b')
			cpage = (cpage == 0) ? cpage : cpage-1;
		else if (nav == 'q')
			break;
	}
}

void present(PRODUCTSET ps) {
	char *str, nav, onav;
	int i, cpage = 1, totalPages, cont=0, total = getPSetSize(ps) ;
	PRODUCT p;

	nav = '\n';
	onav = '\n';

	totalPages = (total / LINES_NUM) + (total % LINES_NUM != 0);

	while (cpage <= totalPages) {

		putchar('\n');
		system("clear");

		printf(":::::::::::: PÁGINA %d de %d ::::::::::::\n", cpage, totalPages);

		for (i=0; i < LINES_NUM && cont < total; cont++, i++) {
			p = getPSetData(ps, cpage * LINES_NUM + i);

			str = fromProduct(p);

			printf("\t\t%s\n", str);

 	/*		freeProduct(p); */
			free(str);
		}
		printf("\nb: Anterior\tn: Seguinte\tq: Sair\th: Ajuda\n\t>> ");

		onav = nav;
		nav = getchar();
		if (nav == '\n') nav = onav;
		else while(getchar() != '\n');

		if (nav == 'n')
			cpage = (cpage == totalPages) ? cpage : cpage+1;
		else if (nav == 'b')
			cpage = (cpage == 0) ? cpage : cpage-1;
		else if (nav == 'q')
			break;
	}
}

/*Devolve numero de comandos executados */
int interpreter() {

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
	printf("      1• Leitura                                         \n");
	printf("      2• Listar Produtos                                 \n");
	printf("      3• Receita do Produto por Mês                      \n");
	printf("      4• Produtos Não Comprados                          \n");
	printf("      5• Gastos de um Cliente                            \n");
	printf("      6• Vendas de Meses                                 \n");
	printf("      7• Clientes Que Compraram em Todas as Filiais      \n");
	printf("      8• Clientes Que Compraram Produto em Filial        \n");
	printf("      9• Produto Mais Compardo por Cliente em Mês        \n");
	printf("     10• Produtos Mais Vendidos em Todo o Ano            \n");
	printf("     11• Três Produtos em que o Cliente mais gastou      \n");
	printf("     12• Clientes sem Compras & Produtos não Vendidos    \n");
	printf("                                                         \n");
	printf("      q• Sair                                            \n");
	printf("                                                         \n");
	printf("  :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");

	printf("\n Escolha uma opção : ");

	fgets(answ, BUFF_SIZE, stdin);

	if (answ[0] == 'q') return 0;

	qnum = atoi(answ);

	switch(qnum) {
		case 1 : query1();
				 break;
		case 2 : query2();
				 break;
		case 3 : query3();
			 	 break;
		case 4 : query4();
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
		default : return interpreter();


	}

	return 1;
}

static void query1() {

}

static void query2() {

}

static void query3() {

}

static void query4() {

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
