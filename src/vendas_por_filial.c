#include <stdlib.h>
#include <string.h>
#include "vendas_por_filial.h"
#include "products.h"
#include "clients.h"
#include "catalog.h"
#include "heap.h"

#define INDEX(i) i - 'A'
#define ALPHA_NUM 26
#define NUM_MONTHS 12
#define NP 2

struct filial {
	CATALOG clients;
};

typedef struct month_list {
	struct product_list *months[NUM_MONTHS];
} *MONTHLIST;

typedef struct product_list {
	HEAP sales;
	double billed;
	int quant;
} *PRODUCTLIST;

typedef struct heap_key {
	char product[PRODUCT_LENGTH];
	int key;
} *HEAPKEY;

typedef struct stock {
	PRODUCT product;
	int quantity[NP];
	double billed[NP];
} *STOCK;

static MONTHLIST initMonthList();
static MONTHLIST addToMonthList(MONTHLIST m, SALE s);
static void freeMonthList(MONTHLIST m);

static PRODUCTLIST initProductList();
static PRODUCTLIST addToProductList(PRODUCTLIST pl, SALE s);
static void freeProductList(PRODUCTLIST pl);

static HEAPKEY createHeapKey(char* product, int key);

static STOCK initStock(); 
static int quantCmp(HEAPKEY k1, HEAPKEY k2);
static STOCK addToStock(STOCK stk, SALE s);

BRANCHSALES initBranchSales () {
	BRANCHSALES bs = malloc(sizeof(*bs));

	bs->clients = initCatalog( ALPHA_NUM,
							   (void* (*) ()) initMonthList,
							   NULL, NULL,
							   (void (*) (void*))freeMonthList);

	return bs;
}

BRANCHSALES addSaleToBranch (BRANCHSALES bs, SALE s) {
	char c[CLIENT_LENGTH];
	MONTHLIST ml;
	CLIENT client = getClient(s);

	fromClient(client, c);

	ml = addCatalog(bs->clients, INDEX(c[0]), c);
	ml = addToMonthList(ml, s);

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

	new->sales = initHeap( (void* (*) ()) initStock,
							(int (*) (void*, void*)) quantCmp,
						  	(void* (*) (void*, void*)) addToStock);
	new->quant = 0;
	new->billed = 0;

	return new;
}

static PRODUCTLIST addToProductList(PRODUCTLIST pl, SALE s) {
	int quant = getQuant(s);
	double billed = quant * getPrice(s);
	char product[PRODUCT_LENGTH];
	PRODUCT p = getProduct(s);
	HEAPKEY hk;

	fromProduct(p, product);
	hk = createHeapKey(product, quant);

	insertHeap(pl->sales, hk, s);

	pl->billed += billed;
	pl->quant += quant;

	return pl;
}

static HEAPKEY createHeapKey(char *product, int key) {
	HEAPKEY new = malloc (sizeof(*new));

	new->key = key;
	strncpy(new->product, product, PRODUCT_LENGTH);

	return new;
}

/* TODO dar uso a esta freeHeapKey */
static void freeHeapKey(HEAPKEY hk) {
	free(hk);
}

static void freeProductList(PRODUCTLIST pl) {
	freeHeap(pl->sales);
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

static int quantCmp(HEAPKEY k1, HEAPKEY k2) {

	int r, sub, q1 = k1->key, q2 = k2->key;

	r = strcmp(k1->product, k2->product);

	/* Só são iguais se o produto for o mesmo */
	if (r != 0)	{
		sub = q1 - q2;
		r = (sub == 0) ? -1 : sub;
	}

	return r;
}

static STOCK addToStock(STOCK stk, SALE s) {
	PRODUCT product = getProduct(s);
	int mode = getMode(s);

	stk->quantity[mode] = getQuant(s);
	stk->billed[mode]   = getPrice(s) * stk->quantity[mode];

	if (isEmptyProduct(stk->product)) stk->product = cloneProduct(product);

	return stk;
}
