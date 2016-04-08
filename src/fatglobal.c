#include <stdlib.h>

#include "catalog.h"
#include "fatglobal.h"

#define CATALOG_SIZE 26

struct faturacao {
	CATALOG l;
};

/**
 * Inicia uma Faturação Global
 * @return Nova Faturação Global
 */
FATGLOBAL initFat() {
	FATGLOBAL fat = malloc(sizeof (*fat));
	fat->l = initCatalog(CATALOG_SIZE);

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

