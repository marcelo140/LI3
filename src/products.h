#ifndef __PRODUCTS__
#define __PRODUCTS__

#include "catalog.h"
#include "generic.h"
#include "set.h"

typedef struct product         *PRODUCT;
typedef struct product_catalog *PRODUCTCAT;

/**
 * Inicializa um catálogo de produtos. Esta estrutura lista todos os produtos existentes
 * indexados pela sua primeira letra.
 */
PRODUCTCAT initProductCat ();

/**
 * Cria uma entrada no catálogo de produtos para o produto indicado.
 */
PRODUCTCAT insertProduct (PRODUCTCAT catalog, PRODUCT product);

/**
 * Liberta todo o espaço ocupado por um catálogo de produtos.
 */
void freeProductCat (PRODUCTCAT catalog);

/**
 * Verifica se o produdo dado existe no catálogo de produtos.
 */
bool lookUpProduct (PRODUCTCAT catalog, PRODUCT product);

/**
 * Calcula o número de produtos começados por uma dada letra existentes no catálogo.
 */
int countProducts (PRODUCTCAT prodCat, char index);

/**
 * Cria uma cópia do catálogo de produtos.
 */
CATALOG getProductCat (PRODUCTCAT prodCatalog);

/**
 * Aloca espaço para um produto.
 */
PRODUCT newProduct();

/**
 * Altera o código do produto dado para a string indicada.
 */
PRODUCT changeProductCode(PRODUCT p, char* str);

/**
 * Cria um novo produto com um código corresponde ao da string dada.
 */
PRODUCT toProduct (char* str);

/**
 * Faz uma cópia do produto dado.
 */
PRODUCT cloneProduct (PRODUCT p);

/**
 * Devolve uma cópia do código do produto.
 */
char* fromProduct (PRODUCT p);

/**
 * Verifica se produto contém algum código associado.
 */
bool isEmptyProduct (PRODUCT p);

/**
 * Liberta todo o espaço ocupado por um produto.
 */
void freeProduct (PRODUCT p);

/**
 * Preenche o conjunto indicado com todos os produtos começados pela letra dada 
 * existentes no catálogo de produtos. 
 */
SET fillProductSet (PRODUCTCAT cat, char index);

#endif
