#include <stdlib.h>
#include <string.h>
#include "vendas_por_filial.h"
#include "products.h"
#include "clients.h"
#include "catalog.h"

#include "avl.h"

#define INDEX(i) i - 'A'
#define ALPHA_NUM 26
#define NUM_MONTHS 12
#define NP 2
#define BRANCHES 3

struct filial {
	CATALOG clients[BRANCHES];
	CATALOG products[BRANCHES];
};

typedef struct month_list {
	struct product_list *months[NUM_MONTHS];
} *MONTHLIST;

typedef struct product_list {
	AVL sales;
	double billed;
	int quant;
} *PRODUCTLIST;

typedef struct stock {
	PRODUCT product;
	int quantity[NP];
	double billed[NP];
} *STOCK;

typedef struct clientQuantity {
	char client[CLIENT_LENGTH];
	int quantity;
} *CLIENTQUANT;

static MONTHLIST initMonthList();
static MONTHLIST addToMonthList(MONTHLIST m, SALE s);
static void freeMonthList(MONTHLIST m);

static PRODUCTLIST initProductList();
static PRODUCTLIST addToProductList(PRODUCTLIST pl, SALE s);
static void freeProductList(PRODUCTLIST pl);


static STOCK initStock(); 
static STOCK addToStock(STOCK stk, SALE s);
static void freeStock(STOCK stk); 

static CLIENTQUANT initClientQuantity();
static CLIENTQUANT addToClientQuantity(CLIENTQUANT cq, SALE s); 
static void freeClientQuantity(CLIENTQUANT cq); 

BRANCHSALES initBranchSales () {
	BRANCHSALES bs = malloc(sizeof(*bs));
	int i;
	
	for (i=0; i < BRANCHES; i++) {
		bs->clients[i] = initCatalog(  ALPHA_NUM,
									   (void* (*) ()) initMonthList,
									   NULL, NULL,
									   (void (*) (void*))freeMonthList);
		bs->products[i] = initCatalog( ALPHA_NUM,
			   						   (void* (*) ()) initClientQuantity,
									   NULL, NULL, 
									   (void (*) (void*)) freeClientQuantity); 
	} 
	return bs;
}

BRANCHSALES addSaleToBranch (BRANCHSALES bs, SALE s) {
	char c[CLIENT_LENGTH], p[PRODUCT_LENGTH];
	int branch;
	MONTHLIST ml;
	CLIENTQUANT cq;
	CLIENT client = getClient(s);
	PRODUCT product = getProduct(s);

	fromClient(client, c);
	fromProduct(product, p);

	branch = getBranch(s);

	ml = addCatalog(bs->clients[branch], INDEX(c[0]), c);
	ml = addToMonthList(ml, s);

	cq = addCatalog(bs->clients[branch], INDEX(p[0]), p); 
	cq = addToClientQuantity(cq, s);

	return bs;
}

/*   =========  FUNÇÕES PARA MONTHLIST ========= */

static MONTHLIST initMonthList() {
	MONTHLIST new = malloc (sizeof(*new));
	int i;

	for (i = 0; i < NUM_MONTHS; i++)
		new->months[i] = initProductList();

	return new;
}

static MONTHLIST addToMonthList(MONTHLIST m, SALE s) {
	int month = getMonth(s);
	PRODUCTLIST pl = m->months[month];

	m->months[month] = addToProductList(pl, s);

	return m;
}

static void freeMonthList(MONTHLIST m) {
	int i;

	for (i=0; i < NUM_MONTHS; i++)
		freeProductList(m->months[i]);

	free(m);
}

/*  ==========  FUNÇÕES PARA PRODUCTLIST  =========== */

static PRODUCTLIST initProductList() {
	PRODUCTLIST new = malloc (sizeof(*new));

	new->sales = initAVL( (void* (*) ()) initStock, NULL, NULL, 
						  (void (*) (void*)) freeStock);

	new->quant = 0;
	new->billed = 0;

	return new;
}

static PRODUCTLIST addToProductList(PRODUCTLIST pl, SALE s) {
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

static void freeProductList(PRODUCTLIST pl) {
	free(pl);
}

/*  ==========  FUNÇÕES PARA STOCK  =========== */

STOCK initStock() {
	STOCK new = malloc(sizeof(*new));

	new->quantity[0] = 0;
	new->quantity[1] = 0;

	new->billed[0] = 0;
	new->billed[1] = 0;

	new->product = newProduct();

	return new;
}

static STOCK addToStock(STOCK stk, SALE s) {
	PRODUCT product = getProduct(s);
	int mode = getMode(s);

	stk->quantity[mode] += getQuant(s);
	stk->billed[mode]   += getPrice(s) * stk->quantity[mode];

	if (isEmptyProduct(stk->product)) stk->product = cloneProduct(product);

	return stk;
}

static void freeStock(STOCK stk) {
	freeProduct(stk->product);
	free(stk);
}

/*  ==========  FUNÇÕES PARA STOCK  =========== */

static CLIENTQUANT initClientQuantity() {
	CLIENTQUANT new = malloc(sizeof(*new));
	new->client[0] = '\0';
	new->quantity = 0;

	return new;
}
	
static CLIENTQUANT addToClientQuantity(CLIENTQUANT cq, SALE s) {
	int quant = getQuant(s);

	cq->quantity += quant;

	return cq;
}


static void freeClientQuantity(CLIENTQUANT cq) {
	free(cq);
}
