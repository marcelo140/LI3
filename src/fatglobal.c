#include <stdlib.h>
#include <string.h>
#include "catalog.h"
#include "fatglobal.h"

#define CATALOG_SIZE 26
#define BUFFER_SIZE 10

#define INDEX(p) (p[0] - 'A')

struct revenue{
	double billed[MONTHS][BRANCHES][SALEMODE];
	int  quantity[MONTHS][BRANCHES][SALEMODE];
};

struct faturacao {
	CATALOG cat;
};

static REVENUE initRevenue  ();
static REVENUE addSaleToRev (REVENUE r, SALE s);
static REVENUE cloneRevenue (REVENUE r);

static void freeRevenue   (REVENUE r);

static bool isEmptyRev    (REVENUE r);
static bool isNotEmptyRev (REVENUE r);

static double getMonthBilled  (REVENUE r, int month,  double *normal, double *promo);
static double getBranchBilled (REVENUE r, int branch, double *normal, double *promo);
static double getBilledRev    (REVENUE r, int b, int m, double* normal, double* promo);

static int getQuantRev   (REVENUE r, int branch, int month, int* normal, int* promo);
static int getBranchQuant(REVENUE r, int branch, int *normal, int *promo);
static int getMonthQuant (REVENUE r, int month,  int *normal, int *promo);

FATGLOBAL initFat(PRODUCTCAT prodCatalog) {
	FATGLOBAL new = malloc(sizeof (*new));

	new->cat = getProductCat(prodCatalog);
	new->cat = changeCatalogOps(new->cat, (init_t) initRevenue, (clone_t) cloneRevenue, 
                                          (free_t) freeRevenue);

	return new;
}

FATGLOBAL addSaleToFat(FATGLOBAL fat, SALE s) {
	REVENUE rev;
	char prod[PRODUCT_LENGTH];

	fromProduct(getProduct(s), prod);

	rev = getCatContent(fat->cat, INDEX(prod), prod);
	addSaleToRev(rev, s);

	return fat;
}

int getProductDataByMonth(FATGLOBAL fat, PRODUCT p, int month, double b[][2], int q[][2]){

	REVENUE rev;
	char product[PRODUCT_LENGTH];
	double billedN = 0, billedP = 0;
	int branch, quantN = 0, quantP = 0;

	fromProduct(p, product);
	rev = getCatContent(fat->cat, INDEX(product), product);

	for(branch = 0; branch < BRANCHES; branch++) {
		getBilledRev(rev, branch, month, &billedN, &billedP);
		getQuantRev(rev, branch, month, &quantN, &quantP);

		b[branch][MODE_N] = billedN;
		b[branch][MODE_P] = billedP;
		q[branch][MODE_N] = quantN;
		q[branch][MODE_P] = quantP;
	}

	return BRANCHES;
}

double getBilledByMonthRange(FATGLOBAL fat, int initialMonth, int finalMonth) {
	SET set;
	REVENUE rev;
	int i, month, size;
	double res = 0;

	set = filterCat(fat->cat, (condition_t) isNotEmptyRev, NULL);
	size = getSetSize(set);

	for(i = 0; i < size; i++){
		rev = getSetData(set, i);

		for(month = initialMonth; month <= finalMonth; month++)
			res += getMonthBilled(rev, month, NULL, NULL);	
	}

	return res;
}

int getQuantByMonthRange(FATGLOBAL fat, int initialMonth, int finalMonth) {
	SET set;
	REVENUE rev;
	int i, month, size, res = 0;

	set = filterCat(fat->cat, (condition_t) isNotEmptyRev, NULL);
	size = getSetSize(set);

	for(i = 0; i < size; i++){
		rev = getSetData(set, i);

		for(month = initialMonth; month <= finalMonth; month++)
			res += getMonthQuant(rev, month, NULL, NULL);	
	}

	return res;
}

SET getProductsSold(FATGLOBAL fat) {
	SET set;

	set = filterCat(fat->cat, (condition_t) isNotEmptyRev, NULL);

	return set;
}

SET getProductsNotSold(FATGLOBAL fat) {
	SET set;

	set = filterCat(fat->cat, (condition_t) isEmptyRev, NULL);	
	
	return set;
}

SET* getProductsNotSoldByBranch(FATGLOBAL fat) {
	SET *res, set;
	REVENUE rev;
	int i, branch, size;

	res = malloc(sizeof(SET) * BRANCHES);

	size = countAllElems(fat->cat);
	set = initSet(size);
	set = fillAllSet(fat->cat, set);

	for(branch = 0; branch < BRANCHES; branch++){
		res[branch] = initSet(10000);
	}

	for(i = 0; i < size; i++) {
		rev = getSetData(set, i);

		for(branch = 0; branch < BRANCHES; branch++){
			if (!getBranchQuant(rev, branch, NULL, NULL))
				datacpy(res[branch], set, i);
		}
	}

	return res;
}

void freeFat(FATGLOBAL fat) {
	if (fat){
		freeCatalog(fat->cat);
		free(fat);
	}
}

/************************** REVENUE *****************************/

static REVENUE initRevenue() {
	return calloc (1, sizeof(struct revenue));
}

static REVENUE addSaleToRev(REVENUE r, SALE s) {
	int quant  = getQuant(s);
	int month  = getMonth(s); 
	int branch = getBranch(s);
	int mode   = getMode(s);
	double billed = quant*getPrice(s);

	r->billed[month][branch][mode]   += billed;
	r->quantity[month][branch][mode] += quant;

	return r;
}

static REVENUE cloneRevenue(REVENUE r) {
	REVENUE new = malloc(sizeof(*new));
	
	memcpy(new->quantity, r->quantity, MONTHS*BRANCHES*SALEMODE*sizeof(int));
	memcpy(new->billed, r->billed,     MONTHS*BRANCHES*SALEMODE*sizeof(double));

	return new;
}

static bool isEmptyRev (REVENUE r) {
	return (r == NULL);
}

static bool isNotEmptyRev (REVENUE r) {
	return (r != NULL);
}

static double getMonthBilled(REVENUE r, int month, double *normal, double *promo) {
	double n = 0, p = 0;
	int branch;

	if (!r)
		return 0;

	for(branch = 0; branch < BRANCHES; branch++) {
		n += r->billed[month][branch][MODE_N];
		p += r->billed[month][branch][MODE_P];
	}

	if (promo)  *promo  = p;
	if (normal) *normal = n;

	return n+p;		
}

static double getBranchBilled(REVENUE r, int branch, double *normal, double *promo) {
	double n = 0, p = 0;
	int  month;

	if (!r)
		return 0;

	for(month = 0; month < MONTHS; month++){
		n += r->billed[month][branch][MODE_N];
		p += r->billed[month][branch][MODE_P];
	}

	if (promo)  *promo  = p;
	if (normal) *normal = n;

	return n+p;
}

static int getQuantRev(REVENUE r, int branch, int month, int* normal, int* promo) {
	int n, p;

	if (!r)
		return 0;

	n = r->quantity[month][branch][MODE_N];
	p = r->quantity[month][branch][MODE_P];

	if (normal) *normal = n;
	if (promo) *promo = p;

	return n+p;	
}

static double getBilledRev(REVENUE r, int b, int m, double* normal, double* promo) {
	double n, p;

	if (!r)
		return 0;

	n = r->billed[m][b][MODE_N];
	p = r->billed[m][b][MODE_P];

	if (normal) *normal = n;
	if (promo) *promo = p;

	return n+p;
}

static int getMonthQuant(REVENUE r, int month, int *normal, int *promo) {
	int n = 0, p = 0, branch;

	if (!r)
		return 0;

	for(branch = 0; branch < BRANCHES; branch++) {
		n += r->quantity[month][branch][MODE_N];
		p += r->quantity[month][branch][MODE_P];
	}

	if (promo)  *promo  = p;
	if (normal) *normal = n;

	return n+p;
}

static int getBranchQuant(REVENUE r, int branch, int *normal, int *promo) {
	int n = 0, p = 0, month;

	if (!r)
		return 0;

	for(month = 0; month < MONTHS; month++) {
		n += r->quantity[month][branch][MODE_N];
		p += r->quantity[month][branch][MODE_P];
	}

	if (promo)  *promo  = p;
	if (normal) *normal = n;

	return n+p;
}

static void freeRevenue(REVENUE r) {
	free(r);
}
