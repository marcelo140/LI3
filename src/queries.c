#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queries.h"
#include <time.h>
#include "set.h"
#define UPPER(a) (('a' <= (a) && (a) <= 'z') ? ((a - 'a') + 'A') : (a))
#define MAX_SIZE 128
#define BRANCHES 3
#define NP 2
#define LINE_NUMS 20

struct page{
	char* header;
	char** list;
	int linesNum;
	int page;
	int totalPages;
	int readH;
	int writeH;
};

static PAGE createPage(char* header, int linesNum,int page, int totalPage);
static PAGE addLineToPage(PAGE p, char* line);
static PAGE getPage(PAGE p, LIST lines, int page);
static int presentList(PAGE page, char* onav);
static void printHelp();
static void freePage(PAGE p);

static PRODUCT askProduct(PRODUCTCAT pcat);
static int askMode();
static int askBranch(); 
static int askBranchPrev(int p1, int p2, int p3); 
static int askMonth(); 
static int askMonthRange(int* begin, int* end); 

void query2(PRODUCTCAT pcat) {
	PAGE page;
	LIST l;
	char aux[MAX_SIZE];
	int size, newPage;

	do {
		printf("Letra: ");
		fgets(aux, MAX_SIZE, stdin);
	} while(aux[0] < 'A' && aux[0] > 'Z');

	l = fillProductSet(pcat, aux[0]);

	size = getListSize(l) / LINE_NUMS;

	strcpy(aux, "\n");
	newPage = 1;
	while(newPage != -1) {
		page = createPage("",LINE_NUMS, newPage, size);
		page = getPage(page, l, newPage);
		if (page) newPage = presentList(page, aux);
		freePage(page);
	}

	freeList(l);
}

void query3(FATGLOBAL fat, PRODUCTCAT pcat) {
	PAGE page; 
	PRODUCT product;
	PRODUCTFAT pfat;
	char answ[MAX_SIZE], oldCmd[MAX_SIZE];
	int i, month, mode, newPage=1, quantT, qtt[BRANCHES][NP];
	double billedT, billed[BRANCHES][NP];

	product = askProduct(pcat);
	if (!product) return;
	mode = askMode();
	if (mode == -1) return;
	month = askMonth();
	if (month == -1) return;

	pfat = getProductDataByMonth(fat, product, month);


	if (mode == 1) {
		quantT  = 0;
		billedT = 0;
		page = createPage("", 2, 1,1);
		
		for (i=0; i < BRANCHES; i++) {
			quantT  += getProductFatQuant(pfat, i, NULL, NULL);
	   		billedT += getProductFatQuant(pfat, i, NULL, NULL);
		}
		
		sprintf(answ, "Quantidade Total: \t%d", quantT);
		page = addLineToPage(page, answ);
		sprintf(answ, "Faturação Total: \t%.2f", billedT);
		page = addLineToPage(page, answ);

	} else {
		page = createPage("\t\tFilial 1\tFilial 2\tFilial 3", 4, 1,1);

		for(i=0; i < BRANCHES; i++) {
			getProductFatQuant(pfat, i, &qtt[i][0], &qtt[i][1]);
			getProductFatBilled(pfat, i, &billed[i][0], &billed[i][1]);
		}

		sprintf(answ, "Quantidade N\t %d\t\t %d\t\t %d", qtt[0][0], qtt[1][0], qtt[2][0]);
		page = addLineToPage(page, answ); 
		sprintf(answ, "Quantidade P\t %d\t\t %d\t\t %d", qtt[0][1], qtt[1][1], qtt[2][1]);
		page = addLineToPage(page, answ); 

		page = addLineToPage(page, answ); 

		sprintf(answ, "Faturado N\t %.2f\t\t %.2f\t\t %.2f", billed[0][0], billed[1][0], billed[2][0]);
		page = addLineToPage(page, answ); 
		sprintf(answ, "Faturado P\t %.2f\t\t %.2f\t\t %.2f", billed[0][1], billed[1][1], billed[2][1]);
		page = addLineToPage(page, answ); 
	}

	freeProductFat(pfat);
	
	strcpy(oldCmd, "\n");
	while (newPage != -1)
		newPage = presentList(page, oldCmd);
}

void query4(FATGLOBAL fat) {
	PAGE page;
	LIST *pgroupB, pgroup;
	char oldCmd[MAX_SIZE];
	int mode, size, sizes[BRANCHES], newPage;

	mode = askMode();
	if (mode == -1) return;


	if (mode == 1) {
		pgroup = getProductsNotSold(fat);
		size  = getListSize(pgroup);
	} else {
		pgroupB = getProductsNotSoldByBranch(fat);
	
		sizes[0] = getListSize(pgroupB[0]);
		sizes[1] = getListSize(pgroupB[1]);
		sizes[2] = getListSize(pgroupB[2]);

		mode = askBranchPrev(sizes[0], sizes[1], sizes[2]);
		if (mode == -1) return;

		size =  sizes[mode];
		pgroup = pgroupB[mode];	
	}
	
	strcpy(oldCmd, "\n");
	newPage=1;
	while(newPage != -1) {
		page = createPage("", LINE_NUMS, newPage, size / LINE_NUMS);
		page = getPage(page, pgroup, newPage);
		if (page) newPage = presentList(page, oldCmd);
		else newPage = -1;
		freePage(page);
	}




/*	PAGE page;
	LIST *pgroupB, pgroup;
	SET auxSet;
	char buff[MAX_SIZE], *line;
	int i, j, op, size=1, aux, newPage;

	op = askMode();

	if (op == 1) {
		pgroup = getProductsNotSold(fat);
		size    = getListSize(pgroup);
		page    = createPage("", LINE_NUMS, 1, size/LINE_NUMS);

	} else {
		pgroupB = getProductsNotSoldByBranch(fat);
		
		for(i=0; i < BRANCHES; i++) {
			aux = getListSize(pgroupB[i]);
			size = (aux > size) ? aux : size;
		}
		
		page = createPage("\tFilial 1\tFilial 2\tFilial 3", LINE_NUMS, 1, size/LINE_NUMS);
	
		auxSet = initSet(size);

		for(i=0; i < size; i++) {
			buff[0] = '\0';
			for(j=0; j < BRANCHES; j++) {
				line = getListElement(pgroupB[j], i);
				if (line) sprintf(buff, "%s\t%s", buff, line);
				else sprintf(buff, "%s\t\t", buff);
			}
			printf("%s\n", buff);
			auxSet = insertElement(auxSet, buff, NULL);
		}
		pgroup = toList(auxSet);
		freeSet(auxSet);
	}

	strcpy(buff, "\n");
	newPage = 1;
	while(newPage != -1) {
		page = getPage(page, pgroup, newPage);
		if (page) newPage = presentList(page, buff);
		else newPage = -1;
		freePage(page);
	}

	freeList(pgroup); */
}

void query5(BRANCHSALES* bs) {
/*	PRINTSET print = initPrintSet(12);
	char str[MAX_SIZE];
	int i, *quantity;

	quantity = getClientQuant(bs, client);

	sprintf(str, "\tMÊS\t    QUANTIDADE");
	print = setPrintHeader(print, str);

	for(i=0; i < 12; i++){
		sprintf(str, "\t%2d\t\t%3d", i+1, quantity[i]);
		print = addToPrintSet(print, str);
	}

	free(quantity); */
}

void query6(FATGLOBAL fat) {
/*
	PRINTSET print = initPrintSet(2);
	double billed;
	int quantity;
	char buff[MAX_SIZE];
	quantity = getQuantByMonthRange(fat, initialMonth, finalMonth);
	billed   = getBilledByMonthRange(fat, initialMonth, finalMonth);
	sprintf(buff, "Quantidade:\t%d", quantity);
	print = addToPrintSet(print, buff);
	sprintf(buff, "Faturado:\t%.2f", billed);
	print = addToPrintSet(print, buff);
*/

}

void query8(BRANCHSALES* bs) {
/*
	PRINTSET print = NULL;
	SET n = initSet(1024), p = initSet(1024), toPrint;
	char answ[MAX_SIZE], *buff;
	int mode = 0, i;

	filterClientsByProduct(bs, product, n, p);

	printf("\n::::::::::::::::::::::::::::::\n\n");
	printf(" 1• Clientes em modo N (%d)\n", getSetSize(n));
	printf(" 2• Clientes em modo P (%d)\n", getSetSize(p));
	printf("\n::::::::::::::::::::::::::::::\n");

	while (mode <= 0 || mode >= 3) {
		printf("Escolha um modo: ");
		fgets(answ, MAX_SIZE, stdin);
		mode = atoi(answ);

		if (answ[0] == 'q') break;

		if (UPPER(answ[0]) == 'N')
			mode = 1;
		else if (UPPER(answ[0]) == 'P')
			mode = 2;
	}

	if (mode == 1 || mode == 2) {
		toPrint = (mode == 1) ? n : p;
		print = initPrintSet(MAX_SIZE);

		for(i=0; (buff = getSetHash(toPrint, i)) ; i++) {
			sprintf(answ, "\t\t%s",buff);
			print = addToPrintSet(print, answ);
			free(buff);
		}
	}

	freeSet(n);
	freeSet(p);
 */
}

void query10(BRANCHSALES* bs) {
/*	PRINTSET print = initPrintSet(n);
	PRODUCTDATA *pdata;
	char buff[MAX_SIZE], *product;
	int i, clients, quantity;
	int max;

	clock_t inicio, fim;
	double tempo;

	inicio = clock();

	pdata = getAllContent(bs, &max);

	if (max < n)
		n = max;

	sprintf(buff, "\t\tPRODUTO\t C    Q");
	print = setPrintHeader(print, buff);

	for(i=0; i < n ; i++) {
		product  = getNameFromProductData(pdata[i]);
		clients  = getClientsFromProductData(pdata[i]);
		quantity = getQuantFromProductData(pdata[i]);

		sprintf(buff, "%5dº \t%s\t%2d  %4d", i+1, product, clients, quantity );
		print = addToPrintSet(print, buff);

		free(product);
	}


	fim = clock();
	tempo = (double) (fim - inicio) / CLOCKS_PER_SEC;

	printf("Tempo: %f\n", tempo);
*/
}

	/*====================== FUNÇÕES DO PRINTSET ===================*/

static PAGE createPage(char* header, int linesNum, int page, int totalPage) {
	PAGE new = malloc (sizeof(*new));

	new->header 	= malloc((strlen(header) + 1) * sizeof(char));
	new->list   	= calloc(linesNum, sizeof(char *));
	new->linesNum 	= linesNum;
	new->page   	= page;
	new->totalPages = totalPage;
	new->readH 		= -1;
	new->writeH 	= -1;

	strcpy(new->header, header);

	return new;
}

static PAGE getPage(PAGE p, LIST lines, int page) {
	int i, index;
	char *line;

	index = (page-1) * p->linesNum;

	for(i=0; i < p->linesNum; i++) {
		line = getListElement(lines, i + index);
		if (line) addLineToPage(p, line);
		else return NULL;
	   free(line);
	}


	return p;
}

static PAGE addLineToPage(PAGE p, char* line) {

	if (p->writeH < p->linesNum - 1) {
		p->writeH++;
		p->list[p->writeH] = calloc(strlen(line) + 1, sizeof(char));
		strcpy(p->list[p->writeH], line);
	}

	return p;
}

int getPageSize(PAGE p) {
	return p->linesNum;
}

char* getNextLine(PAGE p) {
	char *line = NULL, *original;

	if (p->readH < p->writeH) {
		p->readH++;
		original = p->list[p->readH];

		line = malloc((strlen(original)+1) * sizeof(char));
		strcpy(line, original);
	}

	return line;
}

static void freePage(PAGE p) {
	int i;

	if (p) {
		free(p->header);

		for(i=0; i <= p->writeH; i++)
			free(p->list[i]);
		free(p->list);
	}
}

static int presentList(PAGE p, char *ocmd) {
	char *buff, option[MAX_SIZE];
	int i, cpage = p->page, totalPages = p->totalPages, jmp=1;

	option[0] = '\n';

	system("clear");
	putchar('\n');

	printf("::::::::::::::::::::: PÁGINA %d de %d :::::::::::::::::::::\n\n", cpage, totalPages);

	if (p->header[0] != '\0') printf("  %s\n", p->header);

	for(i=0; i < p->linesNum ; i++) {
		buff = getNextLine(p);
		printf("  %s\n", buff);
		free(buff);
	}

	printf(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
	printf("\n b: Anterior\tn: Seguinte\th: Ajuda\n g: Ir para página\tq: Sair\n\t>> ");

	option[0] = getchar();
	/* TODO breaka o PC todo com o n1000 */

	if (option[0] == '\n') strcpy(option, ocmd);
	else fgets(option+1, MAX_SIZE-1, stdin);

	if(option[1] > '0' && option[1] < '9') jmp = atoi(option+1);

	strcpy(ocmd, option);
	switch(option[0]) {
		case 'n': i = cpage + jmp;
				  i = (i > totalPages) ? totalPages : i;
				  break;
		case 'b': i = cpage - jmp;
				  i = (i <= 0) ? 1 : i;
				  break;
		case 'g': i = jmp;
				  i = (i <= 0) ? 1 : i;
				  i = (i > totalPages) ? totalPages : i;
				  break;
		case 'q': i = -1;
				  break;
		case 'h': printHelp();
				  getchar();
		default : i = cpage;
				  break;
	}

	return i;
}

static void printHelp() {
	printf("\tb<num>  Retrocede <num> páginas.\n");
	printf("\tn<num>  Avança <num> páginas.\n");
	printf("\tg<num>  Salta para a página número <num>.\n");
	printf("\t<enter> Utiliza o último comando.\n\t");
}

/* ================ ASKS ===================== */

/* devolve -1 se o utilizador sair */
static int askBranch() {
	char buff[MAX_SIZE];
	int r=0;

	printf("\n  ::::::::::::::::::::::::::::::\n");
	printf("\t   1• Filial 1\n");
	printf("\t   2• Filial 2\n");
	printf("\t   3• Filial 3\n");
	printf("  ::::::::::::::::::::::::::::::\n");
	do {
		printf("  Escolha uma filial: ");
		fgets(buff, MAX_SIZE, stdin);
		r = atoi(buff);
	} while(buff[0] != 'q' && (r < 1 || r > 3));

	return r-1;
}

static int askBranchPrev(int p1, int p2, int p3) {
	char buff[MAX_SIZE];
	int r=0;

	printf("\n  ::::::::::::::::::::::::::::::\n");
	printf("\t   1• Filial 1 (%d)\n", p1);
	printf("\t   2• Filial 2 (%d)\n", p2);
	printf("\t   3• Filial 3 (%d)\n", p3);
	printf("  ::::::::::::::::::::::::::::::\n");
	do {
		printf("  Escolha uma filial: ");
		fgets(buff, MAX_SIZE, stdin);
		r = atoi(buff);
	} while(buff[0] != 'q' && (r < 1 || r > 3));

	return r-1;

}

/* devovle NULL se o utilizador sair */
static PRODUCT askProduct(PRODUCTCAT pcat) {
	PRODUCT product=NULL;
	char buff[MAX_SIZE];
	int stop=0;

	while(!stop) {
		printf("  Produto: ");
		fgets(buff, MAX_SIZE, stdin);
		strtok(buff, "\n\r");
		product = toProduct(buff);

		if (lookUpProduct(pcat, product)) stop=1;
		else if (buff[0] == 'q') {
			stop=1;
			freeProduct(product);
			product = NULL;
		} else if (buff[0] != '\n') printf("Produto Inválido!\n");
	}

	return product;
}

static int askMode() {
	char answ[MAX_SIZE];
	int mode=0;

	printf("\n::::::::::::::::::::::::::::::\n\n");
	printf(" 1• Total\n");
	printf(" 2• Filial a Filial\n");
	printf("\n::::::::::::::::::::::::::::::\n");

	while(mode != 1 && mode != 2) {
		printf("  Escolha um modo: ");
		fgets(answ, MAX_SIZE, stdin);
		mode = atoi(answ);
		if (answ[0] == 'q') return -1;
	}

	return mode;
}

/* devolve -1 se o utilizador sair*/
static int askMonth() {
	char buff[MAX_SIZE];
	int mes;

	while(1) {
		printf("  Mês (1-12): ");
		fgets(buff, MAX_SIZE, stdin);
		mes = atoi(buff);
		if (mes > 0 && mes < 13) break;
		if (buff[0] == 'q') return -1;
		printf("Um mês deve estar entre 1 e 12.");
	}

	return mes;
}

/* devolve -1 caso o utilizador sair */
static int askMonthRange(int* begin, int* end) {
	char buff[MAX_SIZE];
	int b,e, r = 0;

	while(1) {
		printf("  Mês incial (1-12): ");
		fgets(buff, MAX_SIZE, stdin);
		if (buff[0] == 'q') return -1;
		b = atoi(buff);
		if (b >= 1 && b <= 12) break;
		printf("Mês deve estar entre 1 e 12.\n");
	}

	while(1) {
		printf("  Mês incial (%d-12): ", b);
		fgets(buff, MAX_SIZE, stdin);
		if (buff[0] == 'q') return -1;
		e = atoi(buff);
		if (e >= b && e <= 12) break;
		printf("Mês deve estar entre %d e 12.\n", b);
	}

	*begin = b;
	*end   = e;

	return r;
}
