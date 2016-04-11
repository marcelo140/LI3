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

struct prodset {
	CATSET set;	
}; 

/** 
 * Inicia o Catálogo de Produtos
 * @return Catálogo de Produtos novo
 */
PRODUCTCAT initProductCat(){
	PRODUCTCAT prodCat = malloc(sizeof(struct prodcat));
	prodCat->cat = initCatalog(26, NULL, NULL, NULL, NULL, NULL);

	return prodCat;
}

/** 
 * Insere produto no Catálogo de Produtos.
 * @param catalog Catálogo de Produtos
 * @param product Produto a inserir
 * @return Catálogo de Produtos com o novo produto inserido.
 */
PRODUCTCAT insertProduct(PRODUCTCAT prodCat, PRODUCT product) {
	prodCat->cat = insertCatalog(prodCat->cat, product->str[0] - 'A', product->str, NULL);
	return prodCat;
}

/** 
 * Verifica se um dado produto existe num dado catálogo
 * @param catalog Catálogo de Produtos
 * @param product Produto a procurar
 * @return true se encontrou, false caso contrário
 */
bool lookUpProduct(PRODUCTCAT prodCat, PRODUCT product) {
	return lookUpCatalog(prodCat->cat, product->str[0] - 'A', product->str);
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

	r->str = malloc(MAX_BUFF);
	strncpy(r->str, str, MAX_BUFF);

	return r;
}

int countProducts(PRODUCTCAT prodCat, char index) {
	return countCatElems(prodCat->cat, index);
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

/**
 * Liberta a memória ocupado por um produto
 * @param prod Produto a ser libertado
 */
void freeProduct(PRODUCT prod) {
	free(prod->str);
	free(prod);
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

/**
 * Inicializa um PRODUCTSET com um tamanho n
 * @param n Tamanho mínimo do PRODUCTSET
 * @return PRODUCTSET inicializado
 */
PRODUCTSET initPSet(int n) {
	PRODUCTSET new = malloc (sizeof (*new));
	new->set = initCatset(n);

	return new;
}

/**
 * @param prodCat Catálogo onde se encontra a informação
 * @param ps Set onde vai ser guardada a informação
 * @param index Index do catálogo onde se encontra a informação pretendida
 * @return Set com a informação pretendida
 */
PRODUCTSET fillPSet(PRODUCTCAT prodCat, PRODUCTSET ps, char index) {
	ps->set = fillCatset(prodCat->cat, ps->set, index - 'A');

	return ps;
}

PRODUCT getPSetData(PRODUCTSET ps, int pos) {
	char *str = getKeyPos(ps->set, pos);

	return toProduct(str);
}

int getPSetSize(PRODUCTSET ps) {
	return getCatsetSize(ps->set);
}

void freePSet(PRODUCTSET ps) {
	freeCatset(ps->set);
	free(ps);
}
