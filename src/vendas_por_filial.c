#include <stdlib.h>
#include <string.h>

#include "vendas_por_filial.h"
#include "products.h"
#include "clients.h"
#include "catalog.h"

#include "avl.h"
#include "hashT.h"

#define INDEX(s)  s[0] - 'A'

#define ALPHA_NUM 26
#define BRANCHES  3
#define MONTHS    12

#define UNUSED 0
#define N      1
#define NP     2
#define P      3

struct branch {
	CATALOG clients;
	CATALOG products;
};

struct product_data {
	char* productCode;
	int quantity;
	int clients;
};

typedef struct month_list {
	double billed[MONTHS];
	int quant[MONTHS];
} *MONTHLIST;


typedef struct product_sale {
	double billed[MONTHS];
	int quantity[MONTHS];
	int saleType;
} *PRODUCTSALE;

typedef struct client_sale {
	MONTHLIST months;
	HASHT products;
} *CLIENTSALE;

static int compareBilledByYear(PRODUCTSALE s1, PRODUCTSALE s2);
static PRODUCTDATA newProductData(char *productName, int quantity, int clients);
static int toProductData(SET set, PRODUCTDATA* pd);
static PRODUCTSALE cloneProductSale(PRODUCTSALE ps);
static CLIENTSALE initClientSale ();
static CLIENTSALE addToClientSale (CLIENTSALE cs, SALE sale);
static void freeClientSale (CLIENTSALE cs);
static int clientIsShopAholic(CLIENTSALE cs, char* product);
static bool existInProductList(CLIENTSALE cs, char* product);
static int compareQuantByMonth(PRODUCTSALE s1, PRODUCTSALE s2, int *month);

static bool isEmptyClientSale(CLIENTSALE cs);
static bool isNotEmptyClientSale(CLIENTSALE cs);
static int partitionPData(PRODUCTDATA *pd, int begin, int end);
static void sortProductData(PRODUCTDATA* pd, int begin, int end);
void swapPData(PRODUCTDATA* pd, int i, int j);
static void* dumpContent(SET hs, CLIENTSALE cs);
static MONTHLIST initMonthList  ();
static MONTHLIST addToMonthList (MONTHLIST ml, SALE s);
static void      freeMonthList  (MONTHLIST m);

static PRODUCTSALE initProductSale  ();
static PRODUCTSALE addToProductSale (PRODUCTSALE ps, SALE sale);
static void        freeProductSale  (PRODUCTSALE ps);


BRANCHSALES initBranchSales () {
	BRANCHSALES new = malloc(sizeof(*new));

	new->clients = NULL;
	new->products = NULL;

	return new;
}


BRANCHSALES fillBranchSales (BRANCHSALES bs, CLIENTCAT client){

	bs->clients = getClientCat(client);
	bs->clients = changeCatalogOps(bs->clients, (init_t) initClientSale,  NULL,
	                                            (free_t) freeClientSale);

	return bs;
}


BRANCHSALES addSaleToBranch (BRANCHSALES bs, SALE s) {
	CLIENTSALE cs;
	char *c;

	c = getClient(s);
	
	cs = getCatContent(bs->clients, INDEX(c), c);
	cs = addToClientSale(cs, s);

	free(c);
	return bs;
}


PRODUCTDATA* getAllContent(BRANCHSALES bs, int *cenas) {
	SET hashSet;
	PRODUCTDATA* pd;
	int size;

	hashSet = initSet(50000);
	pd = malloc(sizeof(PRODUCTDATA) * 200000);

	hashSet = dumpDataCat(bs->clients, hashSet, (void* (*) (void*, void*)) dumpContent);
	sortSetByName(hashSet);
	size = toProductData(hashSet, pd);
	sortProductData(pd, 0, size-1);

	*cenas = size;
	return pd;
}


LIST getClientsWhoBought (BRANCHSALES bs) {
	SET set;

	set = filterCat(bs->clients, (condition_t) isNotEmptyClientSale, NULL);

	return toList(set);
}


SET getClientsWhoHaveNotBought(BRANCHSALES bs) {
	SET cl;

	cl = filterCat(bs->clients, (condition_t) isEmptyClientSale, NULL);

	return cl;
}


int* getClientQuant(BRANCHSALES bs, CLIENT c) {
	CLIENTSALE content;
	char *client;
	int i, *quant;

	client = fromClient(c);
	quant = malloc(sizeof(int) * MONTHS);

	content = getCatContent(bs->clients, INDEX(client), client);

	for(i = 0; i < MONTHS; i++)
		quant[i] = content->months->quant[i];

	free(client);
	return quant;
}


double *getClientExpenses(BRANCHSALES bs, CLIENT c) {
	CLIENTSALE content;
	char *client;
	double *expenses;
	int i;

	client = fromClient(c);
	expenses = malloc(sizeof(double) * MONTHS);

	content = getCatContent(bs->clients, INDEX(client), client);

	for(i = 0; i < MONTHS; i++)
		expenses[i] = content->months->billed[i];

	free(client);
	return expenses;
}

LIST filterProductsByMonth(BRANCHSALES bs, CLIENT c, int month) {
	CLIENTSALE cs;
	SET products;
	char *client;

	client = fromClient(c);
	cs = getCatContent(bs->clients, INDEX(client), client);

	products = initSet(getHashTsize(cs->products));
	products = dumpHashT(cs->products, products);

	sortSet(products,(compare_t) compareQuantByMonth, &month);

	free(client);
	return toList(products);
}

LIST filterProductsByClient(BRANCHSALES bs, CLIENT c) {
	CLIENTSALE cs;
	SET products;
	char *client;

	client = fromClient(c);
	cs = getCatContent(bs->clients, INDEX(client), client);
	
	products = initSet(getHashTsize(cs->products));
	products = dumpHashT(cs->products, products);

	sortSet(products, (compare_t) compareBilledByYear, NULL);

	free(client);
	return toList(products);
}

void filterClientsByProduct(BRANCHSALES bs, PRODUCT prod, SET n, SET p){
	char *product;

	product = fromProduct(prod);

	condSeparateCat(bs->clients, p, n, (condition_t) existInProductList, product,
                                       (compare_t) clientIsShopAholic, product);

	free(product);
}

/********* STATICS **********/

static int compareQuantByMonth(PRODUCTSALE s1, PRODUCTSALE s2, int *month){
	return (s1->quantity[*month] - s2->quantity[*month]);
}

static int compareBilledByYear(PRODUCTSALE s1, PRODUCTSALE s2) {
	int i, s1Total = 0, s2Total = 0;

	for(i = 0; i < MONTHS; i++){
		s1Total += s1->billed[i];
		s2Total += s2->billed[i];
	}

	return s2Total-s1Total;
}

static PRODUCTDATA newProductData(char *productName, int quantity, int clients) {
	PRODUCTDATA new = malloc(sizeof(*new));

	new->productCode = malloc(sizeof(char) * PRODUCT_LENGTH);
	strncpy(new->productCode, productName, PRODUCT_LENGTH);

	new->quantity = quantity;
	new->clients = clients;

	return new;
}

static void* dumpContent(SET hs, CLIENTSALE cs) {
	return dumpHashT(cs->products, hs);
}

/*   =========  FUNÇÕES PARA MONTHLIST ========= */

static MONTHLIST initMonthList() {
	return calloc(1, sizeof(struct month_list));
}

static MONTHLIST addToMonthList(MONTHLIST ml, SALE s) {
	int month = getMonth(s);
	int quant = getQuant(s);
	double billed = getPrice(s) * quant;

	ml->billed[month] += billed;
	ml->quant[month]  += quant;

	return ml;
}

static void freeMonthList(MONTHLIST m) {
	free(m);
}

/*  ==========  FUNÇÕES PARA CLIENTSALE =========== */

static CLIENTSALE initClientSale() {
	CLIENTSALE new = malloc(sizeof(*new));

	new->months = initMonthList();
	new->products = initHashT((init_t) initProductSale,
                              (add_t)  addToProductSale,
                              (clone_t) cloneProductSale,
                              (free_t) freeProductSale);

	return new;
}

static CLIENTSALE addToClientSale(CLIENTSALE cs, SALE sale) {
	char *product;

	product = getProduct(sale);

	cs->months = addToMonthList(cs->months, sale);
	cs->products = insertHashT(cs->products, product, sale);

	free(product);
	return cs;
}

static void freeClientSale(CLIENTSALE cs) {
	if (cs){
		freeMonthList(cs->months);
		freeHashT(cs->products);
		free(cs);
	}
}

char* getNameFromProductData(PRODUCTDATA pd) {
	if(pd)
		return pd->productCode;

	return NULL;
}

static int toProductData(SET set, PRODUCTDATA* pd) {
	PRODUCTSALE ps;
	int size = getSetSize(set);
	int i, j, clients, quant, month;
	char* productName;

	for (i = 0, j = 0; i < size; j++) {
		productName = getSetHash(set, i);
		quant = 0;

		for(clients = 0; i < size && !strcmp(productName, getSetHash(set, i)); clients++, i++){
			ps = getSetData(set, i);

			for(month = 0; month < MONTHS; month++)
				quant += ps->quantity[month];
		}
		pd[j] = newProductData(productName, quant, clients);
	}

	return j;
}
int getQuantFromProductData(PRODUCTDATA pd) {
	if (pd)
		return pd->quantity;

	return 0;
}

int getClientsFromProductData(PRODUCTDATA pd) {
	if (pd)
		return pd->clients;

	return 0;
}

void swapPData(PRODUCTDATA* pd, int i, int j) {
	PRODUCTDATA tmp = pd[i];
	pd[i] = pd[j];
	pd[j] = tmp;
}
static bool isEmptyClientSale(CLIENTSALE cs) {
	return (cs == NULL);
}

static bool isNotEmptyClientSale(CLIENTSALE cs) {
	return (cs != NULL);
}

static bool existInProductList(CLIENTSALE cs, char* product) {
	return (cs && cs->products && getHashTcontent(cs->products, product) != NULL);
}

static int clientIsShopAholic(CLIENTSALE cs, char* product) {
	PRODUCTSALE ps = getHashTcontent(cs->products, product);

	return (NP - ps->saleType);
}
/*  ==========  FUNÇÕES PARA PRODUCTSALE =========== */

static PRODUCTSALE initProductSale() {
	return calloc(1, (sizeof(struct product_sale)));
}

static PRODUCTSALE addToProductSale(PRODUCTSALE ps, SALE sale) {
	int quant, month, saleType;
	double billed;

	month = getMode(sale);
	quant = getQuant(sale);
	billed = getPrice(sale) * quant;
	saleType = getMode(sale);

	if (saleType == MODE_N)
		saleType = N;
	else
		saleType = P;

	ps->quantity[month] += quant;
	ps->billed[month]   += billed;

	switch (ps->saleType) {
		case UNUSED: ps->saleType = saleType;
					 break;

		case N:      ps->saleType = (saleType == P) ? NP : N;
                     break;

        case P:      ps->saleType = (saleType == N) ? NP : P;
	}

	return ps;
}

static void freeProductSale(PRODUCTSALE ps) {
	free(ps);
}

void freeBranchSales (BRANCHSALES bs) {
	freeCatalog(bs->clients);
	free(bs);
}

static int partitionPData(PRODUCTDATA *pd, int begin, int end) {
	PRODUCTDATA pivot = pd[end];
	int i = begin-1, j;

	for(j = begin; j < end; j++) {
		if (pd[j]->quantity >= pivot->quantity) {
			i++;
			swapPData(pd, i, j);
		}
	}

	swapPData(pd, i+1, end);
	return i+1;
}

static void sortProductData(PRODUCTDATA* pd, int begin, int end) {
	if (begin < end) {
		int q = partitionPData(pd,begin, end);

		sortProductData(pd, begin, q-1);
		sortProductData(pd, q+1, end);
	}
}

PRODUCTSALE cloneProductSale(PRODUCTSALE ps) {
	PRODUCTSALE new = malloc(sizeof(*new));

	memcpy(new->billed, ps->billed, sizeof(double) * MONTHS);
	memcpy(new->quantity, ps->quantity, sizeof(int) * MONTHS);
	new->saleType = ps->saleType;

	return new;
}
