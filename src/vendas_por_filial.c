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

static PRODUCTDATA newProductData(char *productName, int quantity, int clients);
static int toProductData(SET set, PRODUCTDATA* pd);
static PRODUCTSALE cloneProductSale(PRODUCTSALE ps);
static CLIENTSALE initClientSale  ();
static CLIENTSALE addToClientSale (CLIENTSALE cs, SALE sale);
static void       freeClientSale  (CLIENTSALE cs);
static int clientIsShopAholic(CLIENTSALE cs, char* product);
static bool existInProductList(CLIENTSALE cs, char* product);

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
	CLIENT client;
	PRODUCT product;
	char c[CLIENT_LENGTH], p[PRODUCT_LENGTH];

	client  = getClient(s);
	product = getProduct(s);

	fromClient(client, c);
	fromProduct(product, p);

	cs = getCatContent(bs->clients, INDEX(c), c);
	cs = addToClientSale(cs, s);

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


SET getClientsWhoBought (BRANCHSALES bs) {
	SET set;

	set = filterCat(bs->clients, (condition_t) isNotEmptyClientSale, NULL);

	return set;
}


SET getClientsWhoHaveNotBought(BRANCHSALES bs) {
	SET cl;

	cl = filterCat(bs->clients, (condition_t) isEmptyClientSale, NULL);

	return cl;
}


int* getClientQuant(BRANCHSALES bs, CLIENT c) {
	CLIENTSALE content;
	char client[CLIENT_LENGTH];
	int i, *quant;

	fromClient(c, client);
	quant = malloc(sizeof(int) * MONTHS);

	content = getCatContent(bs->clients, INDEX(client), client);

	for(i = 0; i < MONTHS; i++)
		quant[i] = content->months->quant[i];

	return quant;
}


double *getClientExpenses(BRANCHSALES bs, CLIENT c) {
	CLIENTSALE content;
	char client[CLIENT_LENGTH];
	double *expenses;
	int i;

	fromClient(c, client);
	expenses = malloc(sizeof(double) * MONTHS);

	content = getCatContent(bs->clients, INDEX(client), client);

	for(i = 0; i < MONTHS; i++)
		expenses[i] = content->months->billed[i];

	return expenses;
}


void filterClientsByProduct(BRANCHSALES bs, PRODUCT prod, SET n, SET p){
	char product[PRODUCT_LENGTH];

	fromProduct(prod, product);

	condSeparateCat(bs->clients, p, n, (condition_t) existInProductList, product,
                                       (compare_t) clientIsShopAholic, product);
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
	char product[PRODUCT_LENGTH];

	fromProduct(getProduct(sale), product);

	cs->months = addToMonthList(cs->months, sale);
	cs->products = insertHashT(cs->products, product, sale);

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
