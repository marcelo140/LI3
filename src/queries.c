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
static int presentList(char* title, PAGE page, char* onav);
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
	char aux[MAX_SIZE], title[MAX_SIZE];
	int size, newPage;

	do {
		printf("Letra: ");
		fgets(aux, MAX_SIZE, stdin);
		if (aux[0] == 'q') return;
	} while(aux[0] < 'A' || aux[0] > 'Z');

	s = fillProductSet(pcat, aux[0]);

	size = getSetSize(s);
	size = size / LINE_NUMS + ((size % LINE_NUMS != 0) ? 1 : 0);

	sprintf(title, "Query 2  ➤  Produtos com a letra %c (%d).", aux[0], getSetSize(s));
	strcpy(aux, "\n");
	newPage = 1;
	while(newPage != -1) {
		page = createPage("",LINE_NUMS, newPage, size);
		page = getPage(page, s);
		newPage = presentList(title, page, aux);
		freePage(page);
	}

	freeSet(s);
}

void query3(FATGLOBAL fat, PRODUCTCAT pcat) {
	PAGE page;
	PRODUCT product;
	PRODUCTFAT pfat;
	char answ[MAX_SIZE], oldCmd[MAX_SIZE], *pstr;
	int i, month, mode, newPage=1, quantAux[NP], quantT[NP], qtt[BRANCHES][NP];
	double billedAux[NP], billedT[NP], billed[BRANCHES][NP];

	product = askProduct(pcat);
	if (!product) return;
	mode = askMode();
	if (mode == -1) {freeProduct(product); return; } 
	month = askMonth();
	if (month == -1) {freeProduct(product); return;}

	pfat = getProductDataByMonth(fat, product, month);

	if (mode == 1) {
		quantT[0] = 0;
		quantT[1] = 0;
		billedT[0] = 0;
		billedT[1] = 0;

		page = createPage("\t\t\tNORMAL\t\tPROMOÇÃO", 2, 1,1);

		for (i=0; i < BRANCHES; i++) {
			getProductFatSales(pfat, i, &quantAux[0], &quantAux[1]);
	   		getProductFatBilled(pfat, i, &billedAux[0], &billedAux[1]);
			quantT[0] += quantAux[0];
			quantT[1] += quantAux[1];
			billedT[0] += billedAux[0];
			billedT[1] += billedAux[1];
		}
		sprintf(answ, "Total de Vendas:\t%3d\t\t%3d", quantT[0], quantT[1]);
		page = addLineToPage(page, answ);
		sprintf(answ, "Faturação Total:\t%6.2f\t%6.2f", billedT[0], billedT[1]);
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
	
	pstr = fromProduct(product);	
	sprintf(answ, "Query 3  ➤  Receita de %s no mês %d", pstr, month+1);
	strcpy(oldCmd, "\n");
	while (newPage != -1)
		newPage = presentList(answ, page, oldCmd);

	free(pstr);
	freeProduct(product);
	freeProductFat(pfat);
	freePage(page);
}

void query4(FATGLOBAL fat) {
	PAGE page;
	SET *pgroupB, pgroup;
	char oldCmd[MAX_SIZE], title[MAX_SIZE];
	int mode, size, sizes[BRANCHES], newPage;

	mode = askMode();
	if (mode == -1) return;


	if (mode == 1) {
		pgroup = getProductsNotSold(fat);
		size  = getSetSize(pgroup);
		sprintf(title, "Query 4  ➤  Produtos não comprados (%d).", size);
	} else {
		pgroupB = getProductsNotSoldByBranch(fat);

		sizes[0] = getSetSize(pgroupB[0]);
		sizes[1] = getSetSize(pgroupB[1]);
		sizes[2] = getSetSize(pgroupB[2]);

		mode = askBranchPrev(sizes[0], sizes[1], sizes[2]);
		
		if (mode == -1)	return;

		size =  sizes[mode];
		pgroup = pgroupB[mode];
		sprintf(title, "Query 4  ➤  Produtos não comprados da filial %d (%d).", mode+1, size);
	}

	size = size / LINE_NUMS + ((size % LINE_NUMS != 0) ? 1 : 0);
	strcpy(oldCmd, "\n");
	newPage=1;
	while(newPage != -1) {
		page = createPage("", LINE_NUMS, newPage, size);
		page = getPage(page, pgroup);
		if (page) newPage = presentList(title, page, oldCmd);
		else newPage = -1;
		freePage(page);
	}

	freeSet(pgroup);
}

void query5(BRANCHSALES* bs, CLIENTCAT ccat) {
	CLIENT client;
	PAGE page;
	char str[MAX_SIZE], title[MAX_SIZE], *cstr;
	int *quantity1, *quantity2, *quantity3, i, newPage;

	client = askClient(ccat);
	if (!client) return;

	quantity1 = getClientQuantByMonth(bs[0], client);
	quantity2 = getClientQuantByMonth(bs[1], client);
	quantity3 = getClientQuantByMonth(bs[2], client);

	page = createPage("\tMÊS\tFILIAL 1\tFILIAL 2\tFILIAL 3", 12, 1, 1);

	for(i=0; i < 12; i++) {
		sprintf(str, "\t%2d\t  %3d\t\t  %3d\t\t  %3d", i+1, quantity1[i], quantity2[i], quantity3[i]);
		page = addLineToPage(page, str);
	}

	cstr = fromClient(client);
	sprintf(title, "Query 5  ➤  Gastos de %s", cstr);
	strcpy(str, "\n");
	newPage = 1;
	while(newPage != -1) {
		newPage = presentList(title, page, str);
	}

	freePage(page);
	freeClient(client);
	free(quantity1); 
	free(quantity2); 
	free(quantity3); 
	free(cstr);
}

void query6(FATGLOBAL fat) {
	PAGE page;
	double billed;
	int sales, init, final, newPage;
	char buff[MAX_SIZE], title[MAX_SIZE];

	newPage = askMonthRange(&init, &final);
	if (newPage == -1) return;

	page = createPage("", 2, 1, 1);

	sales  = getSalesByMonthRange(fat, init, final);
	billed = getBilledByMonthRange(fat, init, final);
	sprintf(buff, "Vendas:\t%d", sales);
	page = addLineToPage(page, buff);
	sprintf(buff, "Faturado:\t%.2f", billed);
	page = addLineToPage(page, buff);
	
	sprintf(title, "Query 6  ➤  Vendas entre %d e %d", init+1, final+1);
	strcpy(buff, "\n");
	newPage = 1;
	while(newPage != -1) {
		newPage = presentList(title, page, buff);
	}

	freePage(page);
}

void query7(BRANCHSALES* bs) {
	PAGE page;
	SET setB[BRANCHES], tSet, auxSet;
	char ocmd[MAX_SIZE], title[MAX_SIZE];
	int i, size;



	setB[0] = getClientsWhoBought(bs[0]);
	setB[1] = getClientsWhoBought(bs[1]);
	setB[2] = getClientsWhoBought(bs[2]);

	auxSet = intersectSet(setB[0], setB[1]);
	tSet   = intersectSet(auxSet, setB[2]);

	
	size = getSetSize(tSet);
	sprintf(title, "Query 7  ➤  Clientes que compram em todas as filiais (%d).", size);
	size = size / LINE_NUMS + ((size % LINE_NUMS != 0) ? 1 : 0);
	strcpy(ocmd, "\n");
	i = 1;
	while(i != -1) {
		page = createPage("", LINE_NUMS, i, size);
		page = getPage(page, tSet);
		i    = presentList(title, page, ocmd);
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
	char buff[MAX_SIZE], title[MAX_SIZE], *pstr;
	int branch, mode, newPage, size;
	
	product = askProduct(pcat);
	if (!product) return;
	branch = askBranch();
	if (branch == -1) {freeProduct(product); return;} 

	getClientsByProduct(bs[branch], product, &n, &p);
	
	mode = askClientMode(getSetSize(n), getSetSize(p));
	if (mode == -1) {freeProduct(product); freeSet(n); freeSet(p); return; }

	toPrint = (mode == 1) ? n : p;
	size = getSetSize(toPrint);
	size = size / LINE_NUMS + ((size % LINE_NUMS != 0) ? 1 : 0);

	pstr = fromProduct(product);
	sprintf(title, "Query 8  ➤  Clientes que compraram %s na filial %d", pstr, branch+1);
	newPage = 1;
	while (newPage != -1) {
		page = createPage("", LINE_NUMS, newPage, size); 
		page = getPage(page, toPrint);	
		if (page) newPage = presentList(title, page, buff);
		else newPage = -1;
		freePage(page);
	}

	free(pstr);
	freeSet(n);
	freeSet(p);
	freeProduct(product);
}

void query9(BRANCHSALES* bs, CLIENTCAT ccat) {
	PAGE page;
	SET setB[BRANCHES], setT, auxSet, toPrint;
	CLIENT client;
	int i, month, newPage, size;
	char ocmd[MAX_SIZE], title[MAX_SIZE], *line, *cstr;


	client = askClient(ccat);
	if (!client) return;
	month  = askMonth();
	if (month == -1) {freeClient(client); return;} 
	
	setB[0] = getProductsByClient(bs[0], client);
	setB[1] = getProductsByClient(bs[1], client);
	setB[2] = getProductsByClient(bs[2], client);
	
	auxSet = unionSets(setB[0], setB[1]);
	setT   = unionSets(auxSet, setB[2]);

	sortProductListByQuant(setT, month);

	size = getSetSize(setT);

	toPrint = initSet(size, NULL);
	for(i = 0; getClientSetQuantByMonth(setT, i, month) != 0; i++) {
		line    = getSetHash(setT, i);
		toPrint = insertElement(toPrint, line, NULL);
		free(line);
	}

	size = getSetSize(toPrint);

	cstr = fromClient(client);
	sprintf(title, "Query 9  ➤  %d produtos mais comprado por %s no mês %d", size, cstr, month+1);	
	size = size / LINE_NUMS + ((size % LINE_NUMS != 0) ? 1 : 0);

	strcpy(ocmd, "\n");
	newPage = 1;
	while (newPage != -1) {
		page = createPage("", LINE_NUMS, newPage, size);
		page = getPage(page, toPrint);
		newPage = presentList(title, page, ocmd);
		freePage(page);
	}

	free(cstr);
	freeClient(client);
	freeSet(auxSet);
	freeSet(setT);
	freeSet(toPrint);
}

void query10(BRANCHSALES* bs) {
	PAGE page;
	PRODUCTDATA pdata;
	SET s[BRANCHES], l;
	char buff[MAX_SIZE], title[MAX_SIZE], *product;
	char header[]="\t\t\tFILIAL 1\t\tFILIAL 2\t\tFILIAL 3\n\t\t\t PRODUTO C   Q\t|  PRODUTO\t C   Q\t|  PRODUTO\t C   Q";
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
			
			sprintf(buff, "%s\t%s %3d %6d", buff, product, nClients, nQuant);
			free(product);
		}
		l = insertElement(l, buff, NULL);
	}

	size = n / LINE_NUMS + ((n % LINE_NUMS != 0) ? 1 : 0);

	sprintf(title, "Query 10  ➤  %d produtos mais vendidos em todo o ano.", n);	
	i = 1;	
	while(i != -1) {
		page = createPage(header, LINE_NUMS, i, size);
		page = getPage(page, l);		
		i = presentList(title, page, buff);	
		freePage(page);
	}
}

void query11 (BRANCHSALES* bs, CLIENTCAT ccat) {
	PAGE page;
	SET setB[3], setT, auxSet;
	CLIENT client;
	int i;
	char line[MAX_SIZE], title[MAX_SIZE], *product, *cstr;
	double costs;


	client = askClient(ccat);
	if (!client) return;

	setB[0] = getProductsByClient(bs[0], client);
	setB[1] = getProductsByClient(bs[1], client);
	setB[2] = getProductsByClient(bs[2], client);

	auxSet = unionSets(setB[0], setB[1]);
	setT   = unionSets(auxSet, setB[2]);
	
	sortProductListByBilled(setT);
	
	page = createPage("\tPRODUTO\t\tGASTOS\n", 3, 1, 1);
	for(i = 0; i < 3; i++) {
		product = getSetHash(setT, i);
		costs = getClientCosts(setT, i);
		sprintf(line, "\t%s\t\t%6.2f", product, costs );
		page = addLineToPage(page, line);
		free(product);
	}

	cstr = fromClient(client);
	sprintf(title, "Query 11  ➤  3 produtos em que %s mais gastou dinheiro.", cstr);
	strcpy(line, "\n");
	i = 1;
	while(i != -1) 	i = presentList(title, page, line);


	freePage(page);
}

void query12 (BRANCHSALES* bs, FATGLOBAL fat) {
	PAGE page;
	SET clients[3], products, auxSet, clientTSet;
	char line[MAX_SIZE], title[MAX_SIZE] ;
	int i;


	clients[0] = getClientsWhoHaveNotBought(bs[0]);
	clients[1] = getClientsWhoHaveNotBought(bs[1]);
	clients[2] = getClientsWhoHaveNotBought(bs[2]);
	products   = getProductsNotSold(fat);

	auxSet     = intersectSet(clients[0], clients[1]);
	clientTSet = intersectSet(auxSet, clients[2]);

	page = createPage("", 2, 1, 1);
	sprintf(line,"Nº de clientes sem compras:  %6d", getSetSize(clientTSet) );
	page = addLineToPage(page, line);
	sprintf(line,"Nº de produtos não vendidos: %6d", getSetSize(products) );
	page = addLineToPage(page, line);

	sprintf(title, "Query 12  ➤  Clientes sem compras e Produtos não vendidos.");
	strcpy(line, "\n");
	i = 1;
	while(i != -1)
		i = presentList(title, page, line);

	
	freePage(page);
	for(i = 0; i < BRANCHES; i++)
		freeSet(clients[i]);
	freeSet(products);
	freeSet(auxSet);
	freeSet(clientTSet);
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
	char str[MAX_SIZE], *line = NULL;

	index = (p->page-1) * p->linesNum;

	for(i=0; i < p->linesNum; i++) {
		line = getSetHash(lines, i + index);
		sprintf(str, "\t%s", line);
		if (line) addLineToPage(p,str);
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
		free(p);
	}
}

static int presentList(char* title, PAGE p, char *ocmd) {
	char *buff, option[MAX_SIZE];
	int i, cpage = p->page, totalPages = p->totalPages, jmp=1;

	option[0] = '\n';

	system("clear"); 
	
	if(p->totalPages == 0) {
		printf("Não há nada para apresentar!\nPressione qualquer tecla para voltar. ");
		getchar();
		return -1;
	}

	printf("\t%s\n\n", title);

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
	
	while(1) {
		printf("  Filial(1-3): ");
		fgets(buff, MAX_SIZE, stdin);
		r = atoi(buff);
		if (buff[0] == 'q' || (r >= 1 && r <= 3)) break;
		printf("A filial deve estar entre 1 e 3.\n");
	}

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
		if (buff[0] == '\n') continue;
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
		if (buff[0] == '\n') continue;
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

	printf("\n::::::::::::::::::::::::::::::\n");
	printf(" 1• Total\n");
	printf(" 2• Filial a Filial");
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
		printf("  Mês final (%d-12): ", b);
		fgets(buff, MAX_SIZE, stdin);
		if (buff[0] == 'q') return -1;
		e = atoi(buff);
		if (e >= b && e <= 12) break;
		printf("Mês deve estar entre %d e 12.\n", b);
	}

	*begin = b-1;
	*end   = e-1;

	return r;
}
