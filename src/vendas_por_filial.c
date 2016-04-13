#include <stdlib.h>
#include <string.h>
#include "vendas_por_filial.h"
#include "products.h"
#include "catalog.h"
#include "heap.h"

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
	void* key;
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

static STOCK createStock(SALE s);
static int quantCmp(HEAPKEY k1, HEAPKEY k2); 
static STOCK addToStock(STOCK stk, SALE s);

static HEAPKEY createHeapKey(char *product, int* key);

BRANCHSALES initBranchSales () {
	BRANCHSALES bs = malloc(sizeof(*bs));

	bs->clients = initCatalog( ALPHA_NUM, 
							   (void* (*) ()) initMonthList, 
							   (void* (*) (void*, void*)) addToMonthList, 
							   NULL, NULL, 
							   (void (*) (void*))freeMonthList); 

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

	new->sales = initHeap( (void* (*) ()) initHeap,
							(int (*) (void*, void*)) quantCmp, 
						  	(void* (*) (void*, void*)) addToStock);
	new->quant = 0;
	new->billed = 0;

	return new;
}

static PRODUCTLIST addToProductList(PRODUCTLIST pl, SALE s) {
	char product[PRODUCT_LENGTH];
	int quant = getQuant(s);
	int* quantKey = malloc(sizeof(int));
	PRODUCT prod = getProduct(s);
	HEAPKEY hk;
	STOCK stk = createStock(s);
		
	*quantKey = quant;
	fromProduct(prod, product);
	hk = createHeapKey(product, quantKey);

	pl->sales = insertHeap(pl->sales, hk, stk);
	
	return pl;	
} 

static HEAPKEY createHeapKey(char *product, int* key) {
	HEAPKEY new = malloc (sizeof(*new));
	
	new->key = key;
	strncpy(new->product, product, PRODUCT_LENGTH);

	return new;
}

static void freeProductList(PRODUCTLIST pl) {
	
	freeHeap(pl->sales);
	free(pl);
}

/*  ==========  FUNÇÕES PARA STOCK  =========== */

static STOCK initStock() {
	STOCK new = malloc(sizeof(*new));
	
	new->quantity[0] = 0;
	new->quantity[1] = 0;

	new->billed[0] = 0;
	new->billed[1] = 0;

	new->product = newProduct();

	return new;
} 

static int quantCmp(HEAPKEY k1, HEAPKEY k2) {

	int r, *q1 = k1->key, *q2 = k2->key;

	r = *q1 - *q2;

	if (r == 0) {
		/* Só são iguais se o produto for o mesmo */
		if (strcmp(k1->product, k2->product) != 0) r = -1;
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
