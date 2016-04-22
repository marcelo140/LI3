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
static PAGE getPage(PAGE p, SET lines); 
static PAGE resetReadPage(PAGE page); 
static int presentList(PAGE page, char* onav);
static void printHelp();
static void freePage(PAGE p);

static int askClientMode(int n, int p); 
static CLIENT askClient(CLIENTCAT ccat); 
static PRODUCT askProduct(PRODUCTCAT pcat);
static int askMode();
static int askBranch();
static int askBranchPrev(int p1, int p2, int p3);
static int askMonth();
static int askMonthRange(int* begin, int* end);

void query2(PRODUCTCAT pcat) {
	PAGE page;
	SET s;
	char aux[MAX_SIZE];
	int size, newPage;

	do {
		printf("Letra: ");
		fgets(aux, MAX_SIZE, stdin);
		if (aux[0] == 'q') return;
	} while(aux[0] < 'A' && aux[0] > 'Z');

	s = fillProductSet(pcat, aux[0]);

	size = getSetSize(s);
	size = size / LINE_NUMS + ((size % LINE_NUMS != 0) ? 1 : 0);

	strcpy(aux, "\n");
	newPage = 1;
	while(newPage != -1) {
		page = createPage("",LINE_NUMS, newPage, size);
		page = getPage(page, s);
		if (page) newPage = presentList(page, aux);
		freePage(page);
	}

	freeSet(s);
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
			quantT  += getProductFatSales(pfat, i, NULL, NULL);
	   		billedT += getProductFatBilled(pfat, i, NULL, NULL);
		}
		sprintf(answ, "Quantidade Total: \t%d", quantT);
		page = addLineToPage(page, answ);
		sprintf(answ, "Faturação Total: \t%.2f", billedT);
		page = addLineToPage(page, answ);

	} else {
		page = createPage("\t\tFilial 1\tFilial 2\tFilial 3", 5, 1,1);

		for(i=0; i < BRANCHES; i++) {
			getProductFatSales(pfat, i, &qtt[i][0], &qtt[i][1]);
			getProductFatBilled(pfat, i, &billed[i][0], &billed[i][1]);
		}

		sprintf(answ, "Vendas N\t %d\t\t %d\t\t %d", qtt[0][0], qtt[1][0], qtt[2][0]);
		page = addLineToPage(page, answ);

		sprintf(answ, "Vendas P\t %d\t\t %d\t\t %d", qtt[0][1], qtt[1][1], qtt[2][1]);
		page = addLineToPage(page, answ);


		page = addLineToPage(page, "");


		sprintf(answ, "Faturado N\t %6.2f\t %6.2f\t %6.2f", 
								billed[0][0], billed[1][0], billed[2][0]);
		page = addLineToPage(page, answ);

		sprintf(answ, "Faturado P\t %6.2f\t %6.2f\t %6.2f",
			   					billed[0][1], billed[1][1], billed[2][1]);
		page = addLineToPage(page, answ);
	}

	freeProductFat(pfat);

	strcpy(oldCmd, "\n");
	while (newPage != -1)
		newPage = presentList(page, oldCmd);
}

void query4(FATGLOBAL fat) {
	PAGE page;
	SET *pgroupB, pgroup;
	char oldCmd[MAX_SIZE];
	int mode, size, sizes[BRANCHES], newPage;

	mode = askMode();
	if (mode == -1) return;


	if (mode == 1) {
		pgroup = getProductsNotSold(fat);
		size  = getSetSize(pgroup);
	} else {
		pgroupB = getProductsNotSoldByBranch(fat);

		sizes[0] = getSetSize(pgroupB[0]);
		sizes[1] = getSetSize(pgroupB[1]);
		sizes[2] = getSetSize(pgroupB[2]);

		mode = askBranchPrev(sizes[0], sizes[1], sizes[2]);
		if (mode == -1) return;

		size =  sizes[mode];
		pgroup = pgroupB[mode];
	}

	size = size / LINE_NUMS + ((size % LINE_NUMS != 0) ? 1 : 0);

	strcpy(oldCmd, "\n");
	newPage=1;
	while(newPage != -1) {
		page = createPage("", LINE_NUMS, newPage, size / LINE_NUMS);
		page = getPage(page, pgroup);
		if (page) newPage = presentList(page, oldCmd);
		else newPage = -1;
		freePage(page);
	}
}

void query5(BRANCHSALES* bs, CLIENTCAT ccat) {
	CLIENT client;
	PAGE page;
	char str[MAX_SIZE];
	int branch, *quantity, i, newPage;

	branch = askBranch();
	if (branch == -1) return;
	client = askClient(ccat);
	if (!client) return;

	quantity = getClientQuantByMonth(bs[branch], client);

	page = createPage("\tMÊS\t    QUANTIDADE", 12, 1, 1);

	for(i=0; i < 12; i++) {
		sprintf(str, "\t%2d\t\t%3d", i+1, quantity[i]);
		page = addLineToPage(page, str);
	}

	strcpy(str, "\n");
	newPage = 1;
	while(newPage != -1) {
		newPage = presentList(page, str);
	}

	freePage(page);
	freeClient(client);
	free(quantity); 
}

void query6(FATGLOBAL fat) {
	PAGE page;
	double billed;
	int sales, init, final, newPage;
	char buff[MAX_SIZE];

	askMonthRange(&init, &final);

	page = createPage("", 2, 1, 1);

	sales  = getSalesByMonthRange(fat, init, final);
	billed = getBilledByMonthRange(fat, init, final);
	sprintf(buff, "Vendas:\t%d", sales);
	page = addLineToPage(page, buff);
	sprintf(buff, "Faturado:\t%.2f", billed);
	page = addLineToPage(page, buff);
	
	strcpy(buff, "\n");
	newPage = 1;
	while(newPage != -1) {
		newPage = presentList(page, buff);
	}

	freePage(page);
}

void query7(BRANCHSALES* bs) {
	PAGE page;
	SET setB[BRANCHES], tSet, auxSet;
	char ocmd[MAX_SIZE];
	int i, size;



	setB[0] = getClientsWhoBought(bs[0]);
	setB[1] = getClientsWhoBought(bs[1]);
	setB[2] = getClientsWhoBought(bs[2]);

	auxSet = intersectSet(setB[0], setB[1]);
	tSet   = intersectSet(auxSet, setB[2]);

	
	size = getSetSize(tSet);
	size = size / LINE_NUMS + ((size % LINE_NUMS != 0) ? 1 : 0);

	strcpy(ocmd, "\n");
	i = 1;
	while(i != -1) {
		page = createPage("", LINE_NUMS, i, size);
		page = getPage(page, tSet);
		i    = presentList(page, ocmd);
		freePage(page);
	}

	for(i = 0; i < BRANCHES; i++)
		freeSet(setB[i]);
	freeSet(auxSet);
	freeSet(tSet);
}

void query8(BRANCHSALES* bs, PRODUCTCAT pcat) {
	PAGE page;
	SET n, p, toPrint;
	PRODUCT product;
	char buff[MAX_SIZE];
	int branch, mode, newPage, size;
	
	product = askProduct(pcat);
	if (!product) return;
	branch = askBranch();
	if (branch == -1) return;

	getClientsByProduct(bs[branch], product, &n, &p);
	
	mode = askClientMode(getSetSize(n), getSetSize(p));
	if (mode == -1) return;

	toPrint = (mode == 1) ? n : p;
	size = getSetSize(toPrint);
	size = size / LINE_NUMS + ((size % LINE_NUMS != 0) ? 1 : 0);

	newPage = 1;
	while (newPage != -1) {
		page = createPage("", LINE_NUMS, newPage, size); 
		page = getPage(page, toPrint);	
		if (page) newPage = presentList(page, buff);
		else newPage = -1;
		freePage(page);
	}

	freeSet(n);
	freeSet(p);
}

void query10(BRANCHSALES* bs) {
	PAGE page;
	PRODUCTDATA pdata;
	SET s[BRANCHES], l;
	char buff[MAX_SIZE], *product;
	char header[]="\tFILIAL 1\t\tFILIAL 2\t\tFILIAL 3\n PRODUTO\t C   Q\t|  PRODUTO\t C   Q\t|  PRODUTO\t C   Q";
	int i, j, n=0, size, nClients, nQuant;

	while(n <= 0) {
		printf("Número de produtos: ");
		fgets(buff, MAX_SIZE, stdin);
		n = atoi(buff);
		if (buff[0] == '\n') return;
	}

	s[0] = listProductsByQuant(bs[0]);
	s[1] = listProductsByQuant(bs[1]);
	s[2] = listProductsByQuant(bs[2]);


	size = getSetSize(s[0]);
	size = (size > getSetSize(s[1])) ? size : getSetSize(s[1]);
	size = (size > getSetSize(s[2])) ? size : getSetSize(s[2]);


	n = (n > size) ? size : n;

	l = initSet(n, NULL);

	for(i = 0; i < n; i++) {
		sprintf(buff, "%5dº  ", i+1);
		for (j = 0; j < BRANCHES; j++) {
			product  = getSetHash(s[j], i);
			if (!product) { sprintf(buff, "%s\t\t", buff); 	continue;}
			pdata    = getSetData(s[j], i);
			nClients = getClientsFromData(pdata);
			nQuant   = getQuantFromData(pdata);
			
			sprintf(buff, "%s\t%s %d    %d", buff, product, nClients, nQuant);
			free(product);
		}
		l = insertElement(l, buff, NULL);
	}

	size = n / LINE_NUMS + ((n % LINE_NUMS != 0) ? 1 : 0);
	
	strcpy(buff, "\n");
	i = 1;	
	while(i != -1) {
		page = createPage(header, LINE_NUMS, i, size);
		page = getPage(page, l);		
		i = presentList(page, buff);	/*TODO fazer isto aos outros */
		freePage(page);
	}
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

static PAGE getPage(PAGE p, SET lines) {
	int i, index;
	char *line = NULL;

	index = (p->page-1) * p->linesNum;

	for(i=0; i < p->linesNum; i++) {
		line = getSetHash(lines, i + index);
		if (line) addLineToPage(p, line);
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

static int getPageSize(PAGE p) {
	return p->linesNum;
}

static char* getNextLine(PAGE p) {
	char *line = NULL, *original;

	if (p->readH < p->writeH) {
		p->readH++;
		original = p->list[p->readH];

		line = malloc((strlen(original)+1) * sizeof(char));
		strcpy(line, original);
	}

	return line;
}

static PAGE resetReadPage(PAGE page) {
	page->readH = -1;
	return page;
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
		if(!buff) break; 
		printf("  %s\n", buff);
		free(buff);
	}
	p = resetReadPage(p);

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
	printf("\t1• Filial 1 (%d)\n", p1);
	printf("\t2• Filial 2 (%d)\n", p2);
	printf("\t3• Filial 3 (%d)\n", p3);
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

		if (buff[0] == 'q') {
			stop=1;
			freeProduct(product);
			product = NULL;
		} else if (lookUpProduct(pcat, product)) stop=1;
		  else printf("Produto Inválido!\n");
	}

	return product;
}

/* devovle NULL se o utilizador sair */
static CLIENT askClient(CLIENTCAT ccat) {
	CLIENT client=NULL;
	char buff[MAX_SIZE];
	int stop=0;

	while(!stop) {
		printf("  Cliente: ");
		fgets(buff, MAX_SIZE, stdin);
		strtok(buff, "\n\r");
		client = toClient(buff);

		if (buff[0] == 'q') {
			stop = 1;
			freeClient(client);
			client = NULL;
		} else if (lookUpClient(ccat, client)) stop=1;
		  else printf("Cliente Inválido!\n");
	}

	return client;
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

static int askClientMode(int n, int p) {
	int mode=0;
	char answ[MAX_SIZE];

	printf("\n::::::::::::::::::::::::::::::\n\n");
	printf(" 1• Clientes em modo N (%d)\n", n);
	printf(" 2• Clientes em modo P (%d)\n", p);
	printf("\n::::::::::::::::::::::::::::::\n");

	while (mode <= 0 || mode >= 3) {
		printf("Escolha um modo: ");
		fgets(answ, MAX_SIZE, stdin);
		mode = atoi(answ);

		if (answ[0] == 'q') return -1;

		if (UPPER(answ[0]) == 'N')
			mode = 1;
		else if (UPPER(answ[0]) == 'P')
			mode = 2;
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

	return mes-1;
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
		printf("  Mês fimal (%d-12): ", b);
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
