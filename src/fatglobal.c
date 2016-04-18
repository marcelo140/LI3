#include <stdlib.h>

#include "catalog.h"
#include "fatglobal.h"

#define CATALOG_SIZE 26
#define BUFFER_SIZE 10

#define INDEX(p) (p[0] - 'A')

struct faturacao {
	CATALOG cat;
};

REVENUE initRevenue ();
REVENUE addSale       (REVENUE r, SALE s);
REVENUE addBilled     (REVENUE r, int month, int branch, int MODE, double value);
REVENUE addQuantity   (REVENUE r, int month, int branch, int MODE, int value);
REVENUE updateRevenue (REVENUE r, int month, int branch, int MODE, 
                       double billed, int quantity); 
int compareByQuant(REVENUE rev1, REVENUE rev2);
int compareByBilling(REVENUE rev1, REVENUE rev2);
bool isNotEmptyRev (REVENUE r);
bool isEmptyRev (REVENUE r);
double  getMonthBilled  (REVENUE r, int month,  double *normal, double *promo);
double  getBranchBilled (REVENUE r, int branch, double *normal, double *promo);
int     getMonthQuant   (REVENUE r, int month,  int *normal, int *promo);
int     getBranchQuant  (REVENUE r, int branch, int *normal, int *promo);
void freeRevenue (REVENUE r);

PRODUCTGROUP initProductGroup();

struct revenue{
	double billed[MONTHS][BRANCHES][SALEMODE];
	int  quantity[MONTHS][BRANCHES][SALEMODE];
	double totalBilled;
	int totalQuant;
};

struct product_group {
	CATSET products;
};

FATGLOBAL initFat(PRODUCTCAT p) {
	FATGLOBAL new = malloc(sizeof (*new));
	new->cat = getProductCat(p);
	new->cat = changeCatalogOps(new->cat, (init_t) initRevenue, NULL, 
                                          (free_t) freeRevenue);

	return new;
}

FATGLOBAL addFat(FATGLOBAL fat, SALE s) {
	REVENUE r;
	char prod[PRODUCT_LENGTH];

	fromProduct(getProduct(s), prod);
	r = getCatContent(fat->cat, INDEX(prod), prod);
	addSale(r, s);

	return fat;
}

char* getProductCode(PRODUCTGROUP pg, int pos) {
	if (!pg || !pg->products) return NULL;

	return getKeyPos(pg->products, pos);
}

int getProductDataByMonth(FATGLOBAL fat, PRODUCT prod, int month, double billed[][2], 
                                                                     int quant[][2]){
	REVENUE rev;
	char product[PRODUCT_LENGTH];
	double billedN, billedP;
	int branch, quantN, quantP;

	quantN = quantP = 0;
	billedN = billedP = 0;
	fromProduct(prod, product);
	rev = getCatContent(fat->cat, INDEX(product), product);

	for(branch = 0; branch < BRANCHES; branch++) {
		getMonthBilled(rev, month, &billedN, &billedP);
		getMonthQuant(rev, month, &quantN, &quantP);

		billed[branch][MODE_N] = billedN;
		billed[branch][MODE_P] = billedP;
		quant[branch][MODE_N] = quantN;
		quant[branch][MODE_P] = quantP;
	}

	return BRANCHES;
}

PRODUCTGROUP getProductsSold(FATGLOBAL fat) {
	PRODUCTGROUP pg;
	pg = initProductGroup();

	pg->products = filterCat(fat->cat, (condition_t) isNotEmptyRev, NULL);

	return pg;
}

PRODUCTGROUP getProductsNotSold(FATGLOBAL fat) {
	PRODUCTGROUP pg;
	pg = initProductGroup();

	pg->products = filterCat(fat->cat, (condition_t) isEmptyRev, NULL);	
	
	return pg;
}

PRODUCTGROUP* getProductsNotSoldByBranch(FATGLOBAL fat) {
	PRODUCTGROUP* res;
	CATSET cs;
	REVENUE rev;
	int i, branch, size;

	res = malloc(sizeof(PRODUCTGROUP) * BRANCHES);

	size = countAllElems(fat->cat);
	cs = initCatSet(size);
	cs = fillAllCatSet(fat->cat, cs);

	for(branch = 0; branch < BRANCHES; branch++){
		res[branch] = initProductGroup();
		res[branch]->products = initCatSet(10000);
	}

	for(i = 0; i < size; i++) {
		rev = getContPos(cs, i);

		for(branch = 0; branch < BRANCHES; branch++){
			if (!getBranchQuant(rev, branch, NULL, NULL))
				contcpy(res[branch]->products, cs, i);
		}
	}

	return res;
}

double getBilledByMonthRange(FATGLOBAL fat, int initialMonth, int finalMonth) {
	CATSET cs;
	REVENUE rev;
	int i, month, size;
	double res;

	res = 0;
	cs = filterCat(fat->cat, (condition_t) isNotEmptyRev, NULL);
	size = getCatSetSize(cs);

	for(i = 0; i < size; i++){
		rev = getContPos(cs, i);

		for(month = initialMonth; month <= finalMonth; month++)
			res += getMonthBilled(rev, month, NULL, NULL);	
	}

	return res;
}

int getQuantByMonthRange(FATGLOBAL fat, int initialMonth, int finalMonth) {
	CATSET cs;
	REVENUE rev;
	int i, month, size, res;

	res = 0;
	cs = filterCat(fat->cat, (condition_t) isNotEmptyRev, NULL);
	size = getCatSetSize(cs);

	for(i = 0; i < size; i++){
		rev = getContPos(cs, i);

		for(month = initialMonth; month <= finalMonth; month++)
			res += getMonthQuant(rev, month, NULL, NULL);	
	}

	return res;
}

void freeFat(FATGLOBAL fat) {
	freeCatalog(fat->cat);
	free(fat);
}

void freeProductGroup(PRODUCTGROUP pg) {
	freeCatSet(pg->products);
	free(pg);
}

PRODUCTGROUP initProductGroup() {
	return malloc(sizeof(struct product_group));
}

PRODUCTGROUP sortProductGroup(PRODUCTGROUP pg, int sortMode) {
	if (sortMode == BY_QUANTITY)
		sortCatSet(pg->products, (compare_t) compareByQuant);
	else if (sortMode == BY_BILLING)
		sortCatSet(pg->products, (compare_t) compareByBilling);

	return pg;
}

int compareByQuant(REVENUE rev1, REVENUE rev2) {
	return (rev1->quantity - rev2->quantity);
}

int compareByBilling(REVENUE rev1, REVENUE rev2) {
	return (rev1->billed - rev2->billed);
}

/************************** REVENUE *****************************/

REVENUE initRevenue() {
	return calloc (1, sizeof(struct revenue));
}

REVENUE addSale(REVENUE r, SALE s) {
	int quant  = getQuant(s);
	int month  = getMonth(s); 
	int branch = getBranch(s);
	int mode   = getMode(s);
	double billed = quant*getPrice(s);

	r->billed[month][branch][mode]   += billed;
	r->quantity[month][branch][mode] += quant;
	r->totalQuant += quant;
	r->totalBilled += billed;

	return r;
}

bool isEmptyRev (REVENUE r) {
	return (r == NULL);
}

bool isNotEmptyRev (REVENUE r) {
	return (r != NULL);
}

double getMonthBilled(REVENUE r, int month, double *normal, double *promo) {
	double n, p;
	int branch;

	if (!r)
		return 0;

	n = p = 0;

	for(branch = 0; branch < BRANCHES; branch++) {
		n += r->billed[month][branch][MODE_N];
		p += r->billed[month][branch][MODE_P];
	}

	if (promo)  *promo  = p;
	if (normal) *normal = n;

	return n+p;		
}

double getBranchBilled(REVENUE r, int branch, double *normal, double *promo) {
	double n, p;
	int  month;

	if (!r)
		return 0;

	n = p = 0;

	for(month = 0; month < MONTHS; month++){
		n += r->billed[month][branch][MODE_N];
		p += r->billed[month][branch][MODE_P];
	}

	if (promo)  *promo  = p;
	if (normal) *normal = n;

	return n+p;
}

int getMonthQuant(REVENUE r, int month, int *normal, int *promo) {
	int n, p, branch;

	if (!r)
		return 0;

	n = p = 0;

	for(branch = 0; branch < BRANCHES; branch++) {
		n += r->quantity[month][branch][MODE_N];
		p += r->quantity[month][branch][MODE_P];
	}

	if (promo)  *promo  = p;
	if (normal) *normal = n;

	return n+p;
}

int getBranchQuant(REVENUE r, int branch, int *normal, int *promo) {
	int n, p, month;

	if (!r)
		return 0;

	n = p = 0;

	for(month = 0; month < MONTHS; month++) {
		n += r->quantity[month][branch][MODE_N];
		p += r->quantity[month][branch][MODE_P];
	}

	if (promo)  *promo  = p;
	if (normal) *normal = n;

	return n+p;
}

void freeRevenue(REVENUE r) {
	free(r);
}
