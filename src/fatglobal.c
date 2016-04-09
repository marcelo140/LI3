#include <stdlib.h>

#include "catalog.h"
#include "fatglobal.h"
#include "revenue.h"

#define CATALOG_SIZE 26

struct faturacao {
	CATALOG cat;
};

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

/**
 * Adiciona uma venda à faturação global
 * @param fat Faturação
 * @param s Venda a ser adicionada
 * @return Faturação com a nova venda
 */
FATGLOBAL addFat(FATGLOBAL fat, SALE s) {
	char *prod = fromProduct(getProduct(s));

	fat->cat = updateCatalog(fat->cat, prod[0] - 'A', prod, s);

	return fat;
}
