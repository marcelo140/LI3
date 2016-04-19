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
	SET set;	
}; 

PRODUCTCAT initProductCat(){
	PRODUCTCAT productCat = malloc(sizeof(*productCat));

	productCat->cat = initCatalog(CATALOG_SIZE, NULL, NULL, NULL);

	return productCat;
}

PRODUCTCAT insertProduct(PRODUCTCAT productCat, PRODUCT product) {
	productCat->cat = insertCatalog(productCat->cat, INDEX(product), product->str, NULL);

	return productCat;
}

void freeProductCat(PRODUCTCAT productCat) {
	freeCatalog(productCat->cat);
	free(productCat);
}

bool lookUpProduct(PRODUCTCAT productCat, PRODUCT product) {
	return lookUpCatalog(productCat->cat, INDEX(product), product->str);
}

int countProducts(PRODUCTCAT productCat, char index) {
	return countPosElems(productCat->cat, index-'A');
}

CATALOG getProductCat (PRODUCTCAT productCat) {
	return cloneCatalog(productCat->cat);
}

PRODUCT newProduct() {
	PRODUCT new = malloc(sizeof(struct product));
	
	new->str[0] = '\0';

	return new;
}

PRODUCT writeProduct(PRODUCT product, char* str) {
	strncpy(product->str, str, PRODUCT_LENGTH);
	return product;
}

PRODUCT toProduct(char *str) {
	PRODUCT new;

	new = malloc (sizeof(*new));
	strncpy(new->str, str, PRODUCT_LENGTH-1);

	return new;
}

PRODUCT cloneProduct(PRODUCT product) {
	PRODUCT new;

	new = malloc (sizeof (*new));	
	strncpy(new->str, product->str, PRODUCT_LENGTH);

	return new;
}

char* fromProduct(PRODUCT product, char* dest) {
	strcpy(dest, product->str);
	return dest;
}

bool isEmptyProduct(PRODUCT p) {
	return (p->str[0] == '\0');
}

bool isProduct (char *str){
	return IS_CAPITAL_LETTER(str[0]) && 
           IS_CAPITAL_LETTER(str[1]) &&
	       str[2] == '1'             && 
           IS_NUMBER(str[3])         && 
           IS_NUMBER(str[4])         && 
           IS_NUMBER(str[5]);
}

void freeProduct(PRODUCT product) {
	free(product);
}

PRODUCTSET initProductSet(int n) {
	PRODUCTSET new = malloc (sizeof (*new));
	new->set = initSet(n);

	return new;
}

PRODUCTSET fillProductSet(PRODUCTCAT productCat, PRODUCTSET ps, char index) {
	ps->set = fillSet(productCat->cat, ps->set, index - 'A');

	return ps;
}

int getProductSetSize(PRODUCTSET ps) {
	return getSetSize(ps->set);
}

PRODUCT getProductByPos(PRODUCTSET ps, int pos) {
	char *str = getSetHash(ps->set, pos);
	
	if (str == NULL) 
		return NULL;

	return toProduct(str);
}

void freeProductSet(PRODUCTSET ps) {
	freeSet(ps->set);
	free(ps);
}
