#include <stdlib.h>
#include "vendas_por_filial.h"
#include "catalog.h"
#include "heap.h"

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

typedef struct stock {
	PRODUCT product;
	int quantity[NP];
	double billed[NP];
} *STOCK;

static MONTHLIST initMonthList();

static PRODUCTLIST initProductList(); 

static STOCK createStock(SALE s);
static int quantCmp(int* q1, int* q2); 
static STOCK addToStock(STOCK stk, SALE s);

BRANCHSALES initBranchSales () {
	BRANCHSALES bs = malloc(sizeof(*bs));

/*	bs->clients = initCatalog(26, initMonthList, addToMonthList, NULL, NULL, freeMonthList); */

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

/*  ==========  FUNÇÕES PARA PRODUCTLIST  =========== */

static PRODUCTLIST initProductList() {
	PRODUCTLIST new = malloc (sizeof(*new));

	new->sales = initHeap(
							(int (*) (void*, void*)) quantCmp, 
						  	(void* (*) (void*, void*)) addToStock);
	new->quant = 0;
	new->billed = 0;

	return new;
}

static PRODUCTLIST addToProductList(PRODUCTLIST pl, SALE s) {
	int quant = getQuant(s);
	double billed = quant * getPrice(s);
	

	pl->sales = insertHeap(pl->sales, s);	
} 

/*  ==========  FUNÇÕES PARA STOCK  =========== */

static STOCK initStock() {
	STOCK new = malloc(sizeof(*new));
	
	new->quantity[0] = 0;
	new->quantity[1] = 0;

	new->billed[0] = 0;
	new->billed[1] = 0;

/*TODO struct para comprar quant e verificar se sao iguais com cmp*/
	new->product

	return new;
} 

/* TODO remover isto */
static STOCK createStock(SALE s) {
	STOCK new = malloc(sizeof(*new));
	PRODUCT p = getProduct(s);
	int mode = getMode(s);

	new->quantity[mode] = getQuant(s);
	new->billed[mode]   = getPrice(s) * new->quantity[mode];
	new->product        = cloneProduct(p);
	
	return new;
}

static int quantCmp(int* q1, int* q2) {

	return *q1 - *q2;
}

static STOCK addToStock(STOCK stk, SALE s) {
	PRODUCT p = getProduct(s);
	int mode = getMode(s);

	stk->quantity[mode] = getQuant(s);
	stk->billed[mode]   = getPrice(s) * stk->quantity[mode];
	stk->product        = cloneProduct(p);
	
	return stk;
}
