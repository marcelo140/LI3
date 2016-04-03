#include <string.h>
#include <stdlib.h>
#include "catalog.h"
#include "products.h"

#define MAX_BUFF 10

struct product{
	char *str;
};

/** 
 * Inicia o Catálogo de Produtos
 * @return Catálogo de Produtos novo
 */
PRODUCTCAT initProductCat() {
    return initCatalog();
}

/** 
 * Insere produto no Catálogo de Produtos.
 * @param catalog Catálogo de Produtos
 * @param product Produto a inserir
 * @return Catálogo de Produtos com o novo produto inserido.
 */
PRODUCTCAT insertProduct(PRODUCTCAT catalog, PRODUCT product) {
	return insertCatalog(catalog, product->str);
}

/** 
 * Verifica se um dado produto existe num dado catálogo
 * @param catalog Catálogo de Produtos
 * @param product Produto a procurar
 * @return true se encontrou, false caso contrário
 */
bool lookUpProduct(PRODUCTCAT catalog, PRODUCT product) {
	return lookUpCatalog(catalog, product->str);
}

/** 
 * Liberta o espaço ocupado pelo Catálogo de Produtos
 * @param catalog Catálogo de Produtos
 * @return void
 */
void freeProductCat(PRODUCTCAT catalog) {
	freeCatalog(catalog);
}

/** 
 * Converte String para PRODUCT
 * @param str String a converter
 * @return um novo PRODUCT
 */
PRODUCT toProduct(char *str) {
	PRODUCT r = malloc (sizeof (*r));

/*	if (!isProduct(str))
		return NULL;
*/
	r->str = malloc(MAX_BUFF);
	strncpy(r->str, str, MAX_BUFF);

	return r;
}

bool isProduct (char *str){
	int i; 
	bool product;

	product = 1;

	for (i = 0; product && i < 6; i++){
		switch (i){
			case 0:
			case 1: product = (str[i] >= 'A' && str[i] <= 'Z');
					break;
			case 2: product = (str[i] == '1');
					break;
			case 3:
			case 4:
			case 5: product = (str[i] >= '0' && str[i] <= '9');
					break;
			case 6: product = (str[i] == '\n');
		}
	}

	return product;
}
