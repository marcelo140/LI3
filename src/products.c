#include <string.h>
#include <stdlib.h>
#include "catalog.h"
#include "products.h"

#define MAX_BUFF 10

struct product{
	char *str;
};

struct prodcat {
	CATALOG cat;
};

/** 
 * Inicia o Catálogo de Produtos
 * @return Catálogo de Produtos novo
 */
PRODUCTCAT initProductCat(){
	PRODUCTCAT prodCat = malloc(sizeof(struct prodcat));
	prodCat->cat = initCatalog();

	return prodCat;
}

/** 
 * Insere produto no Catálogo de Produtos.
 * @param catalog Catálogo de Produtos
 * @param product Produto a inserir
 * @return Catálogo de Produtos com o novo produto inserido.
 */
PRODUCTCAT insertProduct(PRODUCTCAT prodCat, PRODUCT product) {
	prodCat->cat = insertCatalog(prodCat->cat, product->str, NULL);
	return prodCat;
}

/** 
 * Verifica se um dado produto existe num dado catálogo
 * @param catalog Catálogo de Produtos
 * @param product Produto a procurar
 * @return true se encontrou, false caso contrário
 */
bool lookUpProduct(PRODUCTCAT prodCat, PRODUCT product) {
	return lookUpCatalog(prodCat->cat, product->str);
}

/** 
 * Liberta o espaço ocupado pelo Catálogo de Produtos
 * @param catalog Catálogo de Produtos
 */
void freeProductCat(PRODUCTCAT prodCat) {
	freeCatalog(prodCat->cat);
	free(prodCat);
}

CATALOG prodToCat(PRODUCTCAT prodCat) {
	return prodCat->cat;
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

/**
 * Dado um PRODUCT devolve uma String (Hash) que lhe corresponde
 * @param p PRODUCT
 * @return Hash que lhe corresponde
 */
char* fromProduct(PRODUCT p) {
	char *r = malloc(MAX_BUFF);
	strncpy(r, p->str, MAX_BUFF);
	return r;
}

/**
 * Devolve um clone do um dado PRODUCT
 * @param p PRODUCT a clonar
 * @return PRODUCT novo
 */
PRODUCT cloneProduct(PRODUCT p) {
	PRODUCT new = malloc (sizeof (*new));
	new->str = malloc(MAX_BUFF);
	
	strncpy(new->str, p->str, MAX_BUFF);

	return new;
}

bool isProduct (char *str){
	int i; 
	bool product;

	product = true;

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
