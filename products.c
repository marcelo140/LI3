#include <string.h>
#include <stdlib.h>
#include "products.h"

#define MAX_BUFF 10

struct product{
	char *str;
};

/** Inicia o Catálogo de Produtos
 * @param void
 * @return Catálogo de Produtos novo
 */
PRODUCTCAT initProductCat() {
    return initCatalog();
}

/** Insere produto no Catálogo de Produtos.
 * @param PRODUCTCAT Catálogo de Produtos
 * @param PRODUCT Produto a inserir
 * @return Catálogo de Produtos com o novo produto inserido.
 */
PRODUCTCAT insertProduct(PRODUCTCAT catalog, PRODUCT product) {
	return insertCatalog(catalog, product.str);
}

/** Verifica se um dado produto existe num dado catálogo
 * @param PRODUCTCATALOG Catálogo de Produtos
 * @param PRODUCT Produto a procurar
 * @return 1 se encontrou, 0 caso contrário
 */
bool lookUpProduct(PRODUCTCAT catalog, PRODUCT product) {
	return lookUpCatalog(catalog, product.str);
}

/** Liberta o espaço ocupado pelo Catálogo de Produtos
 * @param PRODUCTCAT Catálogo de Produtos
 * @return void
 */
void freeProductCat(PRODUCTCAT catalog) {
	freeCatalog(catalog);
}

/** Converte String para PRODUCT
 *  @param char* String a converter
 *  @return PRODUCT
 */
PRODUCT toProduct(char *str) {

	PRODUCT r; 
	r.str = malloc(MAX_BUFF);
	strncpy(r.str, str, MAX_BUFF);

	return r;
}
