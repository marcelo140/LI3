#ifndef __PRODUCTS__
#define __PRODUCTS__

#include "catalog.h"
#include "generic.h"

#define PRODUCT_LENGTH 7

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
PRODUCT newProduct ();

/**
 * Altera o código do produto dado para a string indicada.
 */
PRODUCT writeProduct (PRODUCT p, char* str);

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
 * @param p Produto pretendido.
 * @param dest Buffer onde será guardada a cópia do código do produto
 * @return Apontador para dest
 */
char* fromProduct (PRODUCT p, char* dest);

/**
 * Verifica se produto contém algum código associado.
 */
bool isEmptyProduct (PRODUCT p);

/**
 * Verifica se a string dada corresponde a um código de produto válido.
 */
bool isProduct (char* str);

/**
 * Liberta todo o espaço ocupado por um produto.
 */
void freeProduct (PRODUCT p);

/**
 * Aloca espaço para um conjunto de produtos com tamanho inicial n.
 */

/**
 * Preenche o conjunto indicado com todos os produtos começados pela letra dada 
 * existentes no catálogo de produtos. Se o conjunto não tiver espaço suficiente para
 * acatar todos os produtos, este será redimensionado para o tamnha necessário.
 */
LIST fillProductSet (PRODUCTCAT cat, char index);

/**
 * Calcula o tamanho de um conjunto de produtos.
 */

/**
 * Retorna o produto na posição dada do conjunto.
 */

/**
 * Liberta todo o espaço ocupado por um conjunto de produtos.
 */

#endif
