#include <stdlib.h>
#include <string.h>

#include "branchsales.h"
#include "catalog.h"
#include "hashT.h"

#define INDEX(s) s[0]-'A'

#define PRODUCTS_BY_CLIENT 512
#define CLIENTS_BY_PRODUCT 32
#define BRANCHES 3
#define MONTHS 12

#define UNUSED  0
#define SALE_N  1
#define SALE_NP 2
#define SALE_P  3

struct branchsales {
	CATALOG clients;
	CATALOG products;
};

typedef struct client_sale {
	HASHT products;
	int quant[MONTHS];
}*CLIENTSALE ;

typedef struct product_unit {
	double billed[MONTHS];
	int quant[MONTHS];
}*PRODUCTUNIT;

typedef struct product_sale {
	HASHT clients;
	double billed;
	int quantity;
}*PRODUCTSALE;

typedef struct client_unit {
	int saletype;
}*CLIENTUNIT;

static int compareProductUnitByMonth(PRODUCTUNIT pu1, PRODUCTUNIT pu2, int* month);
static int compareProductUnitByBilled(PRODUCTUNIT pu1, PRODUCTUNIT pu2);
bool clientBought(CLIENTSALE cs);
bool clientHaveNotBought(CLIENTSALE cs);
static CLIENTSALE addSaleToClientSale(CLIENTSALE cs, SALE s);
static PRODUCTSALE addSaleToProductSale(PRODUCTSALE ps, SALE s);
static void freeProductSale(PRODUCTSALE ps);
static PRODUCTSALE initProductSale();
static PRODUCTUNIT initProductUnit();
static PRODUCTUNIT addToProductUnit(PRODUCTUNIT product, SALE s);
static PRODUCTUNIT cloneProductUnit(PRODUCTUNIT product);
static void freeProductUnit(PRODUCTUNIT product);
static CLIENTUNIT initClientUnit();
static CLIENTUNIT addToClientUnit(CLIENTUNIT client, SALE s);
static CLIENTUNIT cloneClientUnit(CLIENTUNIT client);
static void freeClientUnit(CLIENTUNIT client);
static CLIENTSALE initClientSale();
static void freeClientSale(CLIENTSALE cs);

BRANCHSALES initBranchSales() {
	BRANCHSALES new = malloc(sizeof(*new));
	
	new->products = NULL;
	new->clients = NULL;

	return new;
}

BRANCHSALES fillBranchSales(BRANCHSALES bs, CLIENTCAT cc, PRODUCTCAT pc) {
	bs->clients = getClientCat(cc);
	bs->products = getProductCat(pc);

	bs->clients = changeCatalogOps(bs->clients, (init_t) initClientSale,
                                                NULL,
                                                (free_t) freeClientSale);


	bs->products = changeCatalogOps(bs->products, (init_t) initProductSale,
                                                  NULL,
                                                  (free_t) freeProductSale);

	return bs;
}

int* getClientQuantByMonth(BRANCHSALES bs, CLIENT c) {
	char* client;
	CLIENTSALE cs;
	int *months;

	client = fromClient(c);
	months = malloc(sizeof(int) * MONTHS);

	cs = getCatContent(bs->clients, INDEX(client), client);
	memcpy(months, cs->quant, sizeof(int) * MONTHS);	

	return months;
}

LIST getClientsWhoBought(BRANCHSALES bs) {
	SET s;

	s = filterCat(bs->clients, (condition_t) clientHaveNotBought, NULL);

	return toList(s);
}

void getClientsByProduct(BRANCHSALES bs, PRODUCT prod, LIST *normal, LIST *promo) {
	PRODUCTSALE ps;
	SET clients, normalClients, promoClients;
	CLIENTUNIT cu;
	char* product;
	int i, size;
	
	product = fromProduct(prod);
	ps = getCatContent(bs->products, INDEX(product), product);

	size = getHashTsize(ps->clients);
	clients = initSet(size);
	normalClients = initSet(size);
	promoClients = initSet(size);

	clients = dumpHashT(ps->clients, clients);

	for(i = 0; i < size; i++){
		cu = getSetData(clients, 0);
	
		switch(cu->saletype) {
			case SALE_N: datacpy(normalClients, clients, i);
                         break;
            case SALE_P: datacpy(promoClients, clients, i);
                         break;
            case SALE_NP: datacpy(normalClients, clients, i);
                          datacpy(promoClients, clients, i);
     	                     break;
		}
	}

	free(product);
	*normal = toList(normalClients);
	*promo = toList(promoClients);
}

LIST getProductsByClient(BRANCHSALES bs, CLIENT c) {
	CLIENTSALE cs;
	SET products;
	char* client;
	int size;
	
	client = fromClient(c);	
	cs = getCatContent(bs->products, INDEX(client), client);

	size = getHashTsize(cs->products);
	products = initSet(size);
	products = dumpHashT(cs->products, products);

	free(client);
	return toList(products);
}

void sortProductListByQuant(LIST productList, int month) {
	sortSet(toSet(productList), (compare_t) compareProductUnitByMonth, &month);
}

void sortProductListByBilled(LIST productList) {
	sortSet(toSet(productList), (compare_t) compareProductUnitByBilled, NULL);
}

void freeBranchSales(BRANCHSALES bs) {
	if (bs) {
		freeCatalog(bs->products);
		freeCatalog(bs->clients);
		free(bs);
	}
}

BRANCHSALES addSaleToBranch(BRANCHSALES bs, SALE s) {
	PRODUCTSALE ps;
	CLIENTSALE cs;
	char *product, *client;

	product = getProduct(s);
	client = getClient(s);

	ps = getCatContent(bs->products, INDEX(product), product);
	cs = getCatContent(bs->clients, INDEX(client), client);

	cs = addSaleToClientSale(cs, s);
	ps = addSaleToProductSale(ps, s);

	return bs;
}

static PRODUCTSALE initProductSale() {
	PRODUCTSALE new = malloc(sizeof(*new));

	new->clients = initHashT(CLIENTS_BY_PRODUCT, (init_t) initClientUnit,
                                                 (add_t) addToClientUnit,
                                                 (clone_t) cloneClientUnit,
                                                 (free_t) freeClientUnit);

	new->quantity = 0;
	new->billed = 0;

	return new;
}

static PRODUCTSALE addSaleToProductSale(PRODUCTSALE ps, SALE s) {
	char* client = getClient(s);
	int quant = getQuant(s);
	int billed = quant*getPrice(s);

	ps->billed += billed;
	ps->quantity += quant;
	ps->clients= insertHashT(ps->clients, client, s);

	free(client);
	return ps;
}

static void freeProductSale(PRODUCTSALE ps) {
	if (ps) {
		freeHashT(ps->clients);
		free(ps);
	}
}

static CLIENTSALE initClientSale() {
	CLIENTSALE new = malloc(sizeof(*new));

	new->products = initHashT(PRODUCTS_BY_CLIENT, (init_t) initProductUnit, 
                                                  (add_t) addToProductUnit,
                                                  (clone_t) cloneProductUnit, 
                                                  (free_t) freeProductUnit);
	
	memset(new->quant, 0, sizeof(int) * MONTHS);

	return new;
}

bool clientBought(CLIENTSALE cs) {
	return (cs == NULL);
}

bool clientHaveNotBought(CLIENTSALE cs) {
	return (cs != NULL);
}

static CLIENTSALE addSaleToClientSale(CLIENTSALE cs, SALE s) {
	char* product = getProduct(s);
	int month = getMonth(s);
	int quant = getQuant(s);
		
	cs->quant[month] += quant;
	cs->products = insertHashT(cs->products, product, s);

	free(product);
	return cs;
}

static void freeClientSale(CLIENTSALE cs) {
	if (cs) {
		freeHashT(cs->products);
		free(cs);
	}
}

static PRODUCTUNIT initProductUnit() {
	return calloc(1, sizeof(struct product_unit));
}

static PRODUCTUNIT addToProductUnit(PRODUCTUNIT product, SALE s) {
	int month = getMonth(s);
	int quant = getQuant(s);
	int billed = quant*getPrice(s);

	product->billed[month] += billed;
	product->quant[month] += quant;

	return product;
}

static PRODUCTUNIT cloneProductUnit(PRODUCTUNIT product) {
	PRODUCTUNIT new = malloc(sizeof(*new));

	memcpy(new->billed, product->billed, sizeof(double) * MONTHS);
	memcpy(new->quant, product->quant, sizeof(int) * MONTHS);

	return new;
}

static int compareProductUnitByMonth(PRODUCTUNIT pu1, PRODUCTUNIT pu2, int* month) {
	return (pu1->quant[*month] - pu2->quant[*month]);
}

static int compareProductUnitByBilled(PRODUCTUNIT pu1, PRODUCTUNIT pu2) {
	double billed1 = 0, billed2 = 0;
	int i;

	for(i = 0; i < MONTHS; i++) {
		billed1 += pu1->billed[i];
		billed2 += pu2->billed[i];
	}

	return (billed2 - billed1);
}

static void freeProductUnit(PRODUCTUNIT product){
	free(product);
}

static CLIENTUNIT initClientUnit() {
	CLIENTUNIT new = malloc(sizeof(*new));
	new->saletype = 0;

	return new;
}

static CLIENTUNIT addToClientUnit(CLIENTUNIT client, SALE s) {
	int mode = (getMode(s) == MODE_N) ? SALE_N : SALE_P;

	switch(client->saletype){
		case UNUSED: client->saletype = mode;
					 break;
		case SALE_N: client->saletype = (mode == SALE_N) ? SALE_N : SALE_NP;
					 break;
		case SALE_P: client->saletype = (mode == SALE_P) ? SALE_P : SALE_NP;
				     break;
	}

	return client;
}

static CLIENTUNIT cloneClientUnit(CLIENTUNIT client) {
	CLIENTUNIT new = malloc(sizeof(*new));
	
	new->saletype = client->saletype;

	return new;
}

static void freeClientUnit(CLIENTUNIT client){
	free(client);
}
