#include <stdlib.h>
#include <string.h>
#include "vendas_por_filial.h"
#include "products.h"
#include "clients.h"
#include "catalog.h"

#include "avl.h"

#define INDEX(p) p[0] - 'A'
#define ALPHA_NUM 26
#define MONTHS 12
#define BRANCHES 3

#define N  1
#define NP 2
#define P  3

struct branch {
	CATALOG clients;
	CATALOG products;
};

typedef struct month_list {
	struct content_list *months[MONTHS];
} *MONTHLIST;

typedef struct content_list {
	AVL sales;
	double billed;
	int quant;
} *CONTENTLIST;

typedef struct stock {
	int quantity;
	double billed;
} *STOCK;

typedef struct saleType {
	int saleType;
} *SALETYPE;

static MONTHLIST initMonthList  ();
static MONTHLIST addToMonthList (MONTHLIST m, SALE s);
static void      freeMonthList  (MONTHLIST m);

static CONTENTLIST initProductList  ();
static CONTENTLIST initClientList  ();
static CONTENTLIST addToProductList (CONTENTLIST cl, SALE s);
static CONTENTLIST addToClientList  (CONTENTLIST cl, SALE s);
static void        freeContentList  (CONTENTLIST cl);

static STOCK initStock  (); 
static STOCK addToStock (STOCK stk, SALE s);
static void  freeStock  (STOCK stk); 

static SALETYPE initSaleType  ();
static SALETYPE addToSaleType (SALETYPE st, SALE s); 
static void     freeSaleType  (SALETYPE st); 

BRANCHSALES initBranchSales (CLIENTCAT clientCat) {
	BRANCHSALES bs = malloc(sizeof(*bs));
	
	bs->clients = getClientCat(clientCat);
	bs->clients = changeCatalogOps(bs->clients,
                                   (cat_init_t) initMonthList,
                                   NULL, NULL,
                                   (cat_free_t) freeMonthList);

	return bs;
}

BRANCHSALES addSaleToBranch (BRANCHSALES bs, SALE s) {
	MONTHLIST ml;
	CONTENTLIST cl;
	CLIENT client;
	PRODUCT product;
	char c[CLIENT_LENGTH], p[PRODUCT_LENGTH];

	client  = getClient(s);
	product = getProduct(s);

	fromClient(client, c);
	fromProduct(product, p);

	ml = getCatContent(bs->clients, INDEX(c), c);
	ml = addToMonthList(ml, s);

	return bs;
}

/*   =========  FUNÇÕES PARA MONTHLIST ========= */

static MONTHLIST initMonthList() {
	MONTHLIST new = malloc (sizeof(*new));
	int i;

	for (i = 0; i < MONTHS; i++)
		new->months[i] = initProductList();

	return new;
}

static MONTHLIST addToMonthList(MONTHLIST m, SALE s) {
	int month = getMonth(s);
	CONTENTLIST cl = m->months[month];

	m->months[month] = addToProductList(cl, s);

	return m;
}

/*TODO if not null */
static void freeMonthList(MONTHLIST m) {
	int i;

	for (i=0; i <MONTHS; i++)
		freeContentList(m->months[i]);

	free(m);
}

/*  ==========  FUNÇÕES PARA CONTENTLIST  =========== */

static CONTENTLIST initProductList() {
	CONTENTLIST new = malloc (sizeof(*new));

	new->sales = initAVL( (void* (*) ()) initStock, NULL, NULL, 
						  (void (*) (void*)) freeStock);

	new->quant = 0;
	new->billed = 0;

	return new;
}

static CONTENTLIST initClientList() {
	CONTENTLIST new = malloc (sizeof(*new));

	new->sales = initAVL( (void* (*) ()) initSaleType, NULL, NULL, 
						  (void (*) (void*)) freeSaleType);

	new->quant = 0;
	new->billed = 0;

	return new;
}

static CONTENTLIST addToProductList(CONTENTLIST pl, SALE s) {
	int quant = getQuant(s);
	double billed = quant * getPrice(s);
	char product[PRODUCT_LENGTH];
	PRODUCT p = getProduct(s);
	STOCK stk;

	fromProduct(p, product);

	stk = addAVL(pl->sales, product);
	addToStock(stk, s);

	pl->billed += billed;
	pl->quant += quant;

	return pl;
}

static CONTENTLIST addToClientList(CONTENTLIST pl, SALE s) {
	int quant = getQuant(s);
	double billed = quant * getPrice(s);
	char client[PRODUCT_LENGTH];
	CLIENT c = getClient(s);
	SALETYPE st;

	fromClient(c, client);

	st = addAVL(pl->sales, client);
	addToSaleType(st, s);

	pl->billed += billed;
	pl->quant += quant;

	return pl;
}

static void freeContentList(CONTENTLIST pl) {
	freeAVL(pl->sales);
	free(pl);
}

/*  ==========  FUNÇÕES PARA STOCK  =========== */

STOCK initStock() {
	STOCK new = malloc(sizeof(*new));

	new->quantity = 0;
	new->billed = 0;

	return new;
}

static STOCK addToStock(STOCK stk, SALE s) {
	int quant;
	double billed;

	quant = getQuant(s);
	billed = getPrice(s);

	stk->quantity += quant;
	stk->billed   += quant*billed;

	return stk;
}

static void freeStock(STOCK stk) {
	free(stk);
}

/*  ==========  FUNÇÕES PARA STOCK  =========== */

static SALETYPE initSaleType() {
	SALETYPE new = malloc(sizeof(*new));
	new->saleType = 0;

	return new;
}
	
static SALETYPE addToSaleType(SALETYPE cq, SALE s) {
	int mode = getMode(s);
	
	if ((mode == MODE_N && cq->saleType == P) || (mode == MODE_P && cq->saleType == N))
		cq->saleType = NP;	
	else if (cq->saleType == 0 && mode == MODE_N)
		cq->saleType = N;
	else if (cq->saleType == 0 && mode == MODE_P)
		cq->saleType = P;			

	return cq;
}


static void freeSaleType(SALETYPE cq) {
	free(cq);
}
