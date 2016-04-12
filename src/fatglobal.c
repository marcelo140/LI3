#include <stdlib.h>

#include "catalog.h"
#include "fatglobal.h"

#define CATALOG_SIZE 26

struct faturacao {
	CATALOG cat;
};

struct fatdata {
	double billed[BRANCHES][NP];
	int    quant[BRANCHES][NP];
};

static CATSET* notSoldBranch(CATSET cs);
static CATSET* notSoldTotal(CATSET cs);

/**
 * Preenche a Faturação Global com o catálog de Produtos
 * @param p Catálogo de Produtos
 * @return Faturação Global preenchida
 */
FATGLOBAL initFat(PRODUCTCAT p) {
	FATGLOBAL new = malloc(sizeof (*new));
	new->cat = cloneCat(prodToCat(p), (void* (*)())            initRevenue,
                                      (void* (*)(void*,void*)) addSale,
                                      NULL, NULL,
                                      (void (*) (void*))       freeRevenue);

	return new;
}

FATDATA initFatdata() {
	FATDATA new = malloc(sizeof(*new));

	return new;
}

/**
 * Adiciona uma venda à faturação global
 * @param fat Faturação
 * @param s Venda a ser adicionada
 * @return Faturação com a nova venda
 */
FATGLOBAL addFat(FATGLOBAL fat, SALE s) {
	char *prod = fromProduct(getProduct(s));

	fat->cat = updateCatalog(fat->cat, prod[0] - 'A', prod, s);

	free(prod);
	return fat;
}

FATDATA monthRevenue(FATGLOBAL fat, char *product, int month, int mode) {
	FATDATA f;
	REVENUE r;
	double billedN, billedP;
	int branch, quantN, quantP;

	f = initFatdata();
	r = getCatContent(fat->cat, product[0]-'A', product);

	if (mode == TOTAL) {
		getMonthQuant(r, month, &quantN, &quantP);
		getMonthBilled(r, month, &billedN, &billedP);

		f->quant[0][MODE_N]  = quantN;
		f->quant[0][MODE_P]  = quantP;
		f->billed[0][MODE_N] = billedN;
		f->billed[0][MODE_P] = billedP;
	}else{
		for(branch = 0; branch < BRANCHES; branch++) {
			f->quant[branch][MODE_N] = getQuantity(r, month, branch, MODE_N);
			f->quant[branch][MODE_N] = getQuantity(r, month, branch, MODE_P);
			f->billed[branch][MODE_N] = getBilled(r, month, branch, MODE_N);
			f->billed[branch][MODE_N] = getBilled(r, month, branch, MODE_P);
		}	
	}

	return f;
}

void monthRange(FATGLOBAL fat, int min, int max, int* quantT, double* billedT) {
	CATSET cs;
	REVENUE r;
	double billed;
	int i, quant, month, size;

	size = countAllElems(fat->cat);
	billed = 0;
	quant = 0;

	cs = initCatSet(size);
	cs = allCatSet(fat->cat, cs);

	for(i = 0; i < size; i++) {
		r = getContPos(cs, i);
		
		if (!isEmptyRev(r)){
			for(month = min; month <= max; month++){
				quant += getMonthQuant(r, month, NULL, NULL);
				billed += getMonthBilled(r, month, NULL, NULL);
			}
		}
	}

	*quantT = quant;
	*billedT = billed;
}

CATSET* notSold(FATGLOBAL fat, int mode) {
	CATSET cs = initCatSet(countAllElems(fat->cat));
	cs = allCatSet(fat->cat, cs);

	if (mode == BRANCHES)
		return notSoldBranch(cs);
	else
		return notSoldTotal(cs);
}

static CATSET* notSoldTotal(CATSET cs) {
	CATSET* res;
	REVENUE rev;
	int i, size;

	size = getCatSetSize(cs);
	res = malloc(sizeof(*res));

	res[0] = initCatSet(1000);

	for(i = 0; i < size; i++) {
		rev = getContPos(cs, i);

		if (isEmptyRev(rev))
			contcpy(res[0], cs, i);
	}

	return res;
}

static CATSET* notSoldBranch(CATSET cs) {
	CATSET* res;
	REVENUE rev;
	int i, branch, size;

	size = getCatSetSize(cs);
	res = malloc(sizeof(*res) * BRANCHES);

	for(branch = 0; branch < BRANCHES; branch++)
		res[branch] = initCatSet(1000);

	for(i = 0; i < size; i++){
		rev = getContPos(cs, i);
		
		for(branch = 0; branch < BRANCHES; branch++)
			if (!getBranchQuant(rev, branch, NULL, NULL))
				contcpy(res[branch], cs, i);
		
	}
	
	return res;
}

double getBilledFat(FATDATA data, int branch, double *billedN, double *billedP) {
	*billedN = data->billed[branch][MODE_N];
	*billedP = data->billed[branch][MODE_P];

	return *billedN + *billedP;
}

int getQuantFat(FATDATA data, int branch, int *quantN, int *quantP) {
	*quantN = data->quant[branch][MODE_N];
	*quantP = data->quant[branch][MODE_P];

	return *quantN + *quantP;
}

void freeFatData(FATDATA fd) {
	free(fd->billed);
	free(fd->quant);
	free(fd);
}

void freeFat(FATGLOBAL fat) {
	freeCatalog(fat->cat);
	free(fat);
}
