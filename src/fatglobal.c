#include <stdlib.h>

#include "catalog.h"
#include "fatglobal.h"

struct faturacao {
	CATALOG l;
};

typedef struct fatProduct {
	int totalSales[MONTHS][BRANCHES][NP];
	double billed[MONTHS][BRANCHES][NP];
} *FATPRODUCT;

static FATPRODUCT initFatProduct();

/**
 * Inicia uma Faturação Global
 * @return Nova Faturação Global
 */
FATGLOBAL initFat() {
	FATGLOBAL fat = malloc(sizeof (*fat));
	fat->l = initCatalog(13);

	return fat;
}

/**
 * Preenche a Faturação Global com o catálog de Produtos
 * @param p Catálogo de Produtos
 * @return Faturação Global preenchida
 */
FATGLOBAL fillFat(PRODUCTCAT p) {
	FATGLOBAL new = initFat();
	new->l = cloneCatalog(prodToCat(p), NULL);

	return new;
}

/**
 * Verifica se uma Faturação Global está vazia
 * @param f Faturação Global a verificar
 * @return true caso esteja vazia, false caso contrário
 */
bool isEmptyFat(FATGLOBAL f) {
	return (f==NULL);
}

/************************AVISO NÃO FUNCIONA!                   NÃO FUNCIONA!!!
 * Adiciona uma venda à Faturação Global
 * @param fat Faturação Global
 * @param s SALE que representa a venda
 * @return Faturação Global com a nova venda
 */
FATGLOBAL addFat(FATGLOBAL fat, SALE s) {
	char *p = fromProduct(getProduct(s));
	int month = getMonth(s), branch = getBranch(s), mode = getMode(s);
	double price = getPrice(s);
	FATPRODUCT cntt = getCatContent(fat->l, 0, p);

	if (!cntt) 
		cntt = initFatProduct();
	
	cntt->billed[month-1][branch-1][mode] += price;
	cntt->totalSales[month-1][branch-1][mode]++;

	fat->l = updateCatalog(fat->l, 0, p, cntt);

	return fat;
}

static FATPRODUCT initFatProduct() {
	int i, j, k;
	FATPRODUCT new = malloc (sizeof(struct fatProduct));

	for (i=0; i < MONTHS; i++)
		for (j=0; j < BRANCHES; j++)
			for (k=0; k < NP; k++) {
				new->billed[i][j][k] = 0;
				new->totalSales[i][j][k] = 0;
			}

	return new;
}
