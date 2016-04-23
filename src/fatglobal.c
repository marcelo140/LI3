#include <stdlib.h>
#include <string.h>

#include "catalog.h"
#include "fatglobal.h"

#define INDEX(p) (p[0] - 'A')
#define BRANCHES(p) p->branches

/* Dados de cada produto */
struct revenue{
	double *billed;
	int *sales;
	int branches;
};

/** Dados de um produto num dado mês */
struct product_fat {
	double *billed;
	int *sales;
};

struct faturacao {
	CATALOG cat;
	int branches;
};

static PRODUCTFAT newProductFat(int branches);
static void addProductFatBilled(PRODUCTFAT pf, int branch, double normal, double promo);
static void addProductFatSales(PRODUCTFAT pf, int branch, int normal, int promo);

/* Set de funções que auxiliam a gestão do módulo */
static REVENUE initRevenue  (int branches);
static REVENUE addSaleToRev (REVENUE r, SALE s);
static REVENUE cloneRevenue (REVENUE r);
static void    freeRevenue  (REVENUE r);

/* Predicados para o filter */
static bool isEmptyRev    (REVENUE r);
static bool isNotEmptyRev (REVENUE r);

/* Getters para os dados de cada produto */
static double getMonthBilled  (REVENUE r, int month,  double *normal, double *promo);
static double getBranchBilled (REVENUE r, int branch, double *normal, double *promo);
static double getBilledRev    (REVENUE r, int b, int m, double* normal, double* promo);
static int getSalesRev    (REVENUE r, int branch, int month, int* normal, int* promo);
static int getBranchSales (REVENUE r, int branch, int *normal, int *promo);
static int getMonthSales  (REVENUE r, int month,  int *normal, int *promo);


FATGLOBAL initFat(int branches){
	FATGLOBAL new = malloc(sizeof(*new));

	new->cat = NULL;
	new->branches = branches;

	return new;
}

FATGLOBAL fillFat (FATGLOBAL fat, PRODUCTCAT p) {
	fat->cat = getProductCat(p);
	fat->cat = changeCatalogOps(fat->cat, (clone_t) cloneRevenue, (free_t) freeRevenue);

	return fat;
}

FATGLOBAL addFat(FATGLOBAL fat, SALE s) {
	REVENUE rev;
	MEMBER member = newMember();
	char *prod = getProduct(s);

	rev = getCatContent(fat->cat, INDEX(prod), prod, member);

	if (!rev)
		rev = initRevenue(BRANCHES(fat));

	addSaleToRev(rev, s);
	updateMember(member, rev);	

	freeMember(member);
	free(prod);

	return fat;
}

FATGLOBAL addSaleToFat(FATGLOBAL fat, SALE s) {
	REVENUE rev;
	MEMBER member = newMember();
	char *product = getProduct(s);

	rev = getCatContent(fat->cat, INDEX(product), product, member);

	if (!rev)
		rev = initRevenue(BRANCHES(fat));

	addSaleToRev(rev, s);
	updateMember(member, rev);

	freeMember(member);
	free(product);
	
	return fat;
}

PRODUCTFAT getProductDataByMonth(FATGLOBAL fat, PRODUCT p, int month) {
	REVENUE rev;
	PRODUCTFAT pf = newProductFat(BRANCHES(fat));
	char *product = fromProduct(p);
	double billedN = 0, billedP = 0;
	int branch, salesN = 0, salesP = 0;

	rev = getCatContent(fat->cat, INDEX(product), product, NULL);

	if (!rev){
		free(product);
		return pf;
	}

	for(branch = 0; branch < BRANCHES(fat); branch++) {
		getBilledRev(rev, branch, month, &billedN, &billedP);
		getSalesRev(rev, branch, month, &salesN, &salesP);

		addProductFatSales(pf, branch, salesN, salesP);
		addProductFatBilled(pf, branch, billedN, billedP);
	}
	
	free(product);
	return pf;
}

double getBilledByMonthRange(FATGLOBAL fat, int initialMonth, int finalMonth) {
	SET set;
	REVENUE rev;
	int i, month, size;
	double res = 0;

	set = initSet(countAllElems(fat->cat), (free_t) freeRevenue);

	set = filterCat(fat->cat, set, (condition_t) isNotEmptyRev, NULL);
	size = getSetSize(set);

	for(i = 0; i < size; i++){
		rev = getSetData(set, i);

		for(month = initialMonth; month <= finalMonth; month++)
			res += getMonthBilled(rev, month, NULL, NULL);
	}

	freeSet(set);
	return res;
}

int getSalesByMonthRange(FATGLOBAL fat, int initialMonth, int finalMonth) {
	SET set;
	REVENUE rev;
	int i, month, size, res = 0;
	
	set = initSet(countAllElems(fat->cat), (free_t) freeRevenue);

	set = filterCat(fat->cat, set, (condition_t) isNotEmptyRev, NULL);
	size = getSetSize(set);

	for(i = 0; i < size; i++){
		rev = getSetData(set, i);

		for(month = initialMonth; month <= finalMonth; month++)
			res += getMonthSales(rev, month, NULL, NULL);
	}
	
	freeSet(set);
	return res;
}

SET getProductsNotSold(FATGLOBAL fat) {
	SET set = initSet(countAllElems(fat->cat), (free_t) freeRevenue);

	set = filterCat(fat->cat, set, (condition_t) isEmptyRev, NULL);

	return set;
}

SET* getProductsNotSoldByBranch(FATGLOBAL fat) {
	SET *res, set;
	REVENUE rev;
	int i, branch, size;

	res = malloc(sizeof(SET) * BRANCHES(fat));

	set = initSet(countAllElems(fat->cat), (free_t) freeRevenue);
	set = fillAllSet(fat->cat, set);
	size = getSetSize(set);

	for(branch = 0; branch < BRANCHES(fat); branch++)
		res[branch] = initSet(size, (free_t) freeRevenue);

	for(i = 0; i < size; i++) {
		for(branch = 0; branch < BRANCHES(fat); branch++){
			rev = getSetData(set, i);
			if (!getBranchSales(rev, branch, NULL, NULL))
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

static REVENUE initRevenue(int branches) {
	REVENUE new;
   	new = malloc(sizeof(*new));

	new->branches = branches;
	new->billed   = calloc(MONTHS*branches*SALEMODE, sizeof(double));
	new->sales    = calloc(MONTHS*branches*SALEMODE, sizeof(int));

	return new;
}

static REVENUE addSaleToRev(REVENUE r, SALE s) {
	int quant  = getQuant(s);
	int month  = getMonth(s);
	int branch = getBranch(s);
	int mode   = getMode(s);
	double billed = quant*getPrice(s);

	r->billed[(month * BRANCHES(r) + branch) * SALEMODE + mode] += billed;
	r->sales[(month * BRANCHES(r) + branch) * SALEMODE + mode]++;

	return r;
}

static REVENUE cloneRevenue(REVENUE r) {
	REVENUE new = malloc(sizeof(*new));

	new->sales = malloc(MONTHS*BRANCHES(r)*SALEMODE*sizeof(int));
	new->billed = malloc(MONTHS*BRANCHES(r)*SALEMODE*sizeof(double));

	memcpy(new->sales, r->sales, MONTHS*BRANCHES(r)*SALEMODE*sizeof(int));
	memcpy(new->billed, r->billed, MONTHS*BRANCHES(r)*SALEMODE*sizeof(double));
	new->branches = r->branches;
	
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

	for(branch = 0; branch < BRANCHES(r); branch++) {
		n += r->billed[(month * BRANCHES(r) + branch) * SALEMODE + MODE_N];
		p += r->billed[(month * BRANCHES(r) + branch) * SALEMODE + MODE_P];
	}

	if (promo) *promo  = p;
	if (normal) *normal = n;

	return n+p;
}

static double getBranchBilled(REVENUE r, int branch, double *normal, double *promo) {
	double n = 0, p = 0;
	int  month;

	if (!r)
		return 0;

	for(month = 0; month < MONTHS; month++){
		n += r->billed[(month * BRANCHES(r) + branch) * SALEMODE + MODE_N];
		p += r->billed[(month * BRANCHES(r) + branch) * SALEMODE + MODE_P];
	}

	if (promo)  *promo  = p;
	if (normal) *normal = n;

	return n+p;
}

static int getSalesRev(REVENUE r, int branch, int month, int* normal, int* promo) {
	int n, p;

	if (!r)
		return 0;

	n = r->sales[(month * BRANCHES(r) + branch) * SALEMODE + MODE_N];
	p = r->sales[(month * BRANCHES(r) + branch) * SALEMODE + MODE_P];

	if (normal) *normal = n;
	if (promo) *promo = p;

	return n+p;
}

static double getBilledRev(REVENUE r, int b, int m, double* normal, double* promo) {
	double n, p;

	if (!r)
		return 0;

	n = r->billed[(m * BRANCHES(r) + b) * SALEMODE + MODE_N];
	p = r->billed[(m * BRANCHES(r) + b) * SALEMODE + MODE_P];

	if (normal) *normal = n;
	if (promo) *promo = p;

	return n+p;
}

static int getMonthSales(REVENUE r, int month, int *normal, int *promo) {
	int n = 0, p = 0, branch;

	if (!r)
		return 0;

	for(branch = 0; branch < BRANCHES(r); branch++) {
		n += r->sales[(month * BRANCHES(r) + branch) * SALEMODE + MODE_N];
		p += r->sales[(month * BRANCHES(r) + branch) * SALEMODE + MODE_P];
	}

	if (promo)  *promo  = p;
	if (normal) *normal = n;

	return n+p;
}

static int getBranchSales(REVENUE r, int branch, int *normal, int *promo) {
	int n = 0, p = 0, month;

	if (!r)
		return 0;

	for(month = 0; month < MONTHS; month++) {
		n += r->sales[(month * BRANCHES(r) + branch) * SALEMODE + MODE_N];
		p += r->sales[(month * BRANCHES(r) + branch) * SALEMODE + MODE_P];
	}

	if (promo) *promo  = p;
	if (normal) *normal = n;

	return n+p;
}

static void freeRevenue(REVENUE r) {
	if (r) {
		free(r->sales);
		free(r->billed);
		free(r);	
	}
}

static PRODUCTFAT newProductFat(int branch) {
	PRODUCTFAT new = malloc(sizeof(*new));
	
	new->sales = calloc(SALEMODE*branch, sizeof(int));
	new->billed = calloc(SALEMODE*branch, sizeof(double));

	return new;
}

int getProductFatSales(PRODUCTFAT pf, int branch, int* normal, int* promo) {
	int n, p;

	n = pf->sales[branch*SALEMODE + MODE_N];
	p = pf->sales[branch*SALEMODE + MODE_P];

	if (normal) *normal = n;
	if (promo) *promo  = p;

	return n+p;
}

double getProductFatBilled(PRODUCTFAT pf, int branch, double* normal, double* promo) {
	double n, p;

	n = pf->billed[branch*SALEMODE + MODE_N];
	p = pf->billed[branch*SALEMODE + MODE_P];

	if (normal) *normal = n;
	if (promo) *promo = p;

	return n+p;
}

static void addProductFatBilled(PRODUCTFAT pf, int branch, double normal, double promo) {
	pf->billed[branch*SALEMODE + MODE_N] += normal;
	pf->billed[branch*SALEMODE + MODE_P] += promo;
}

static void addProductFatSales(PRODUCTFAT pf, int branch, int normal, int promo) {
	pf->sales[branch*SALEMODE + MODE_N] += normal;
	pf->sales[branch*SALEMODE + MODE_P] += promo;
}

void freeProductFat(PRODUCTFAT pf) {
	if (pf) {
		free(pf->sales);
		free(pf->billed);
		free(pf);
	}
}
