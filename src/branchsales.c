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

struct product_data {
	int quantity;
	int clients;	
};

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
PRODUCTDATA dumpProductSale(PRODUCTSALE ps);
int compareProductDataByQuant(PRODUCTDATA pd1, PRODUCTDATA pd2);

BRANCHSALES initBranchSales() {
	BRANCHSALES new = malloc(sizeof(*new));
	
	new->products = NULL;
	new->clients = NULL;

	return new;
}

BRANCHSALES fillBranchSales(BRANCHSALES bs, CLIENTCAT cc, PRODUCTCAT pc) {
	bs->clients = getClientCat(cc);
	bs->products = getProductCat(pc);

	bs->clients = changeCatalogOps(bs->clients, NULL,
                                                (free_t) freeClientSale);


	bs->products = changeCatalogOps(bs->products, NULL,
                                                  (free_t) freeProductSale);

	return bs;
}

int* getClientQuantByMonth(BRANCHSALES bs, CLIENT c) {
	char* client;
	CLIENTSALE cs;
	int *months;

	client = fromClient(c);
	cs = getCatContent(bs->clients, INDEX(client), client, NULL);

	months = calloc(MONTHS, sizeof(int));

	if (cs)
		memcpy(months, cs->quant, sizeof(int) * MONTHS);	

	free(client);
	return months;
}

SET getClientsWhoBought(BRANCHSALES bs) {
	SET s = initSet(countAllElems(bs->clients), (free_t) freeClientSale);

	s = filterCat(bs->clients, s, (condition_t) clientBought, NULL);

	return s;
}

SET getClientsWhoHaveNotBought(BRANCHSALES bs) {
	SET s = initSet(countAllElems(bs->clients), (free_t) freeClientSale);
	
	s = filterCat(bs->clients, s, (condition_t) clientHaveNotBought, NULL);

	return s;
}

void getClientsByProduct(BRANCHSALES bs, PRODUCT prod, SET *normal, SET *promo) {
	PRODUCTSALE ps;
	SET clients, normalClients, promoClients;
	CLIENTUNIT cu;
	char* product;
	int i, size;
	
	product = fromProduct(prod);
	ps = getCatContent(bs->products, INDEX(product), product, NULL);

	if (!ps) {
		*normal = initSet(0, NULL);
		*promo = initSet(0, NULL);
		free(product);
		return;
	}

	size = getHashTsize(ps->clients);
	clients = initSet(size, (free_t) freeClientUnit);
	normalClients = initSet(size, (free_t) freeClientUnit);
	promoClients = initSet(size, (free_t) freeClientUnit);

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
	freeSet(clients);

	*normal = normalClients;
	*promo = promoClients;
}

SET getProductsByClient(BRANCHSALES bs, CLIENT c) {
	CLIENTSALE cs;
	SET products;
	char* client;
	int size;
	
	client = fromClient(c);	
	cs = getCatContent(bs->products, INDEX(client), client, NULL);

	if (!cs) {
		products = initSet(0, NULL);
		free(client);
		return products;
	}

	size = getHashTsize(cs->products);
	products = initSet(size, (free_t) freeProductUnit);
	products = dumpHashT(cs->products, products);

	free(client);
	return products;
}

void sortProductListByQuant(SET productList, int month) {
	sortSet(productList, (compare_t) compareProductUnitByMonth, &month);
}

void sortProductListByBilled(SET productList) {
	sortSet(productList, (compare_t) compareProductUnitByBilled, NULL);
}

SET listProductsByQuant(BRANCHSALES bs) {
	SET s = initSet(countAllElems(bs->products), (free_t) freeProductData);

	s = dumpCatalog(bs->products, s, (void*(*)(void*)) dumpProductSale);
	sortSet(s, (compare_t) compareProductDataByQuant, NULL);

	return s;
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
	MEMBER member;
	char *product, *client;

	product = getProduct(s);
	client = getClient(s);
	member = newMember();

	ps = getCatContent(bs->products, INDEX(product), product, member);
	
	if (!ps)
		ps = initProductSale();

	ps = addSaleToProductSale(ps, s);
	updateMember(member, ps);


	cs = getCatContent(bs->clients, INDEX(client), client, member);

	if (!cs)
		cs = initClientSale();

	cs = addSaleToClientSale(cs, s);
	updateMember(member, cs);

	free(product);
	free(client);

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

int compareProductDataByQuant(PRODUCTDATA pd1, PRODUCTDATA pd2){
	return (pd1->quantity - pd2->quantity);
}

PRODUCTDATA dumpProductSale(PRODUCTSALE ps) {
	PRODUCTDATA new = NULL;

	if (ps){
		new = malloc(sizeof(*new));
		new->clients = getHashTsize(ps->clients);
		new->quantity = ps->quantity;
	}

	return new;
}

void freeProductData(PRODUCTDATA pd) {
	free(pd);
}

int getClientsFromData(PRODUCTDATA pd) {
	return pd->clients;
}

int getQuantFromData(PRODUCTDATA pd) {
	return pd->quantity;
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
	return (cs != NULL);
}

bool clientHaveNotBought(CLIENTSALE cs) {
	return (cs == NULL);
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
