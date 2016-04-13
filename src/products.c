#include <string.h>
#include <stdlib.h>
#include "products.h"

#define CATALOG_SIZE 26

#define INDEX(p)             (p->str[0] - 'A')
#define IS_CAPITAL_LETTER(c) (c >= 'A' && c <= 'Z')
#define IS_NUMBER(c)         (c >= '0' && c <= '9')

struct product{
	char str[PRODUCT_LENGTH];
};

struct product_catalog {
	CATALOG cat;
};

struct product_set {
	CATSET set;	
}; 

/** 
 * Inicia o Catálogo de Produtos
 * @return Catálogo de Produtos novo
 */
PRODUCTCAT initProductCat(){
	PRODUCTCAT productCat;

	productCat= malloc(sizeof(*productCat));
	productCat->cat = initCatalog(CATALOG_SIZE, NULL, NULL, NULL, NULL);

	return productCat;
}

/** 
 * Insere produto no Catálogo de Produtos.
 * @param productCat Catálogo de Produtos
 * @param product Produto a inserir
 * @return Catálogo de Produtos com o novo produto inserido.
 */
PRODUCTCAT insertProduct(PRODUCTCAT productCat, PRODUCT product) {
	productCat->cat = insertCatalog(productCat->cat, INDEX(product), product->str, NULL);

	return productCat;
}

/** 
 * Liberta o espaço ocupado pelo Catálogo de Produtos
 * @param productCat Catálogo de Produtos
 */
void freeProductCat(PRODUCTCAT productCat) {
	freeCatalog(productCat->cat);
	free(productCat);
}

/** 
 * Verifica se um dado produto existe num dado catálogo
 * @param productCat Catálogo de Produtos
 * @param product Produto a procurar
 * @return true se encontrou, false caso contrário
 */
bool lookUpProduct(PRODUCTCAT productCat, PRODUCT product) {
	return lookUpCatalog(productCat->cat, INDEX(product), product->str);
}

/**
 * Conta o número de produtos no catálogo começados por uma dada letra
 * @param productCat Catálogo
 * @param index Letra
 * @return Número de elementos
 */
int countProducts(PRODUCTCAT productCat, char index) {
	return countPosElems(productCat->cat, index);
}

CATALOG prodToCat(PRODUCTCAT productCat) {
	return productCat->cat;
}

/**
 * @return Retorna um produto não inicializado
 */
PRODUCT newProduct() {
	PRODUCT new = malloc(sizeof(struct product));
	
	new->str[0] = '\0';

	return new;
}

bool isEmptyProduct(PRODUCT p) {
	return (p->str[0] == '\0');
}

/** 
 * Converte str para PRODUCT
 * @param str String a converter
 * @return um novo PRODUCT
 */
PRODUCT toProduct(char *str) {
	PRODUCT new;

	new = malloc (sizeof (*new));
	strncpy(new->str, str, PRODUCT_LENGTH);

	return new;
}

/**
 * Atribui a string dada ao produto
 * @param p Produto a ser alterado
 * @param str String dada
 * @return Produto alterado
 */
PRODUCT writeProduct(PRODUCT product, char* str) {
	strncpy(product->str, str, PRODUCT_LENGTH);
	return product;
}

/**
 * Devolve um clone do um dado PRODUCT
 * @param p PRODUCT a clonar
 * @return PRODUCT novo
 */
PRODUCT cloneProduct(PRODUCT product) {
	PRODUCT new;

	new = malloc (sizeof (*new));	
	strncpy(new->str, product->str, PRODUCT_LENGTH);

	return new;
}

/**
 * Dado um PRODUCT copia o nome do produto para a string dada
 * @param p PRODUCT
 * @param dest String destino
 * @return String destino
 */
char* fromProduct(PRODUCT product, char* dest) {
	strncpy(dest, product->str, PRODUCT_LENGTH);
	return dest;
}

/**
 * Liberta a memória ocupado por um produto
 * @param prod Produto a ser libertado
 */
void freeProduct(PRODUCT product) {
	free(product);
}

bool isProduct (char *str){
	return IS_CAPITAL_LETTER(str[0]) && 
           IS_CAPITAL_LETTER(str[1]) &&
	       str[2] == '1'             && 
           IS_NUMBER(str[3])         && 
           IS_NUMBER(str[4])         && 
           IS_NUMBER(str[5])         && 
           str[6] == 0;
}

/**
 * Inicializa um PRODUCTSET com um tamanho n
 * @param n Tamanho mínimo do PRODUCTSET
 * @return PRODUCTSET inicializado
 */
PRODUCTSET initProductSet(int n) {
	PRODUCTSET new = malloc (sizeof (*new));
	new->set = initCatalogSet(n);

	return new;
}

/**
 * @param productCat Catálogo onde se encontra a informação
 * @param ps Set onde vai ser guardada a informação
 * @param index Index do catálogo onde se encontra a informação pretendida
 * @return Set com a informação pretendida
 */
PRODUCTSET fillProductSet(PRODUCTCAT productCat, PRODUCTSET ps, char index) {
	ps->set = fillCatalogSet(productCat->cat, ps->set, index - 'A');

	return ps;
}

void freeProductSet(PRODUCTSET ps) {
	freeCatalogSet(ps->set);
	free(ps);
}

PRODUCT getProductByPos(PRODUCTSET ps, int pos) {
	char *str = getKeyPos(ps->set, pos);

	return toProduct(str);
}

int getProductSetSize(PRODUCTSET ps) {
	return getCatalogSetSize(ps->set);
}

