#include <stdlib.h>

#include "catalog.h"
#include "fatglobal.h"

#define CATALOG_SIZE 26

struct faturacao {
	CATALOG cat;
};

struct fatdata {
	double* billed[2];
	int*    quant[2];    
	int     size;
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
	new->cat = cloneCat(prodToCat(p), (void* (*)()) initRevenue,
                        (void* (*) (void*,void*)) addSale,
                        NULL, NULL,
                        (void (*) (void*))freeRevenue);

	return new;
}

FATDATA initFatdata(int size) {
	FATDATA new = malloc(sizeof(*new));

	new->size = size;

	new->billed[0] = malloc(size * sizeof(double *));
	new->billed[1] = malloc(size * sizeof(double *));
	
	new->quant[0]  = malloc(size * sizeof(int *));
	new->quant[1]  = malloc(size * sizeof(int *));

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
	int i, quantN, quantP;

	f = initFatdata(mode);
	r = getCatContent(fat->cat, product[0]-'A', product);

	for(i = 0; i < mode; i++){
		getMonthQuant(r, month, &quantN, &quantP);
		getMonthBilled(r, month, &billedN, &billedP);

		f->quant[0][i]  = quantN;
		f->quant[1][i]  = quantP;
		f->billed[0][i] = billedN;
		f->billed[1][i] = billedP;
	}

	return f;
}

CATSET* notSold(FATGLOBAL fat, int mode) {
	CATSET cs = initCatset(countAllElems(fat->cat));
	cs = allCatset(fat->cat, cs);

	if (mode == BRANCHES)
		return notSoldBranch(cs);
	else
		return notSoldTotal(cs);
}

static CATSET* notSoldTotal(CATSET cs) {
	CATSET* res;
	REVENUE rev;
	int i, size;
	
	size = getCatsetSize(cs);
	res = malloc(sizeof(*res));
	
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
	
	size = getCatsetSize(cs);
	res = malloc(sizeof(*res) * BRANCHES);

	for(i = 0; i < size; i++){
		rev = getContPos(cs, i);
		
		if (isEmptyRev(rev)){
			for(branch = 0; branch < BRANCHES; branch++)
				contcpy(res[branch], cs, i);
		}else{
			for(branch = 0; branch < BRANCHES; branch++)
				if (getBranchQuant(rev, branch, NULL, NULL))
					contcpy(res[branch], cs, i);
		}
	}

	return res;
}

void freeFat(FATGLOBAL fat) {
	freeCatalog(fat->cat);
	free(fat);
}
