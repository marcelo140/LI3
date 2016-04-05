#include <stdlib.h>

#include "catalog.h"
#include "fatglobal.h"

struct faturacao {
	CATALOG l;
};

typedef struct fatProduct {
	double billed[MONTHS][BRANCHES][NP];
} *FATPRODUCT;

/**
 * Inicia uma Faturação Global
 * @return Nova Faturação Global
 */
FATGLOBAL initFat() {
		FATGLOBAL fat = malloc(sizeof (*fat));
		fat->l = initCatalog();

		return fat;
}

/**
 * Verifica se uma Faturação Global está vazia
 * @param f Faturação Global a verificar
 * @return true caso esteja vazia, false caso contrário
 */
bool isEmptyFat(FATGLOBAL f) {
	return (f==NULL);
}

/**
 * Adiciona uma venda à Faturação Global
 * @param fat Faturação Global
 * @param s SALE que reprensta a venda
 * @return Faturação Global com a nova venda
 */
FATGLOBAL addFat(FATGLOBAL fat, SALE s) {
	
	char *p = fromProduct(getProduct(s));
	int month = getMonth(s), branch = getBranch(s), mode = getMode(s);
	double price = getPrice(s);
	FATPRODUCT cntt = getCatalogContent(fat->l, p);

	cntt->billed[month][branch][mode] += price;

	return fat;
}
