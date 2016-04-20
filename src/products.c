#include <string.h>
#include <stdlib.h>
#include "products.h"

#define CATALOG_SIZE 26

#define INDEX(p)             (p->str[0] - 'A')

struct product{
	char *str;
};

struct product_catalog {
	CATALOG cat;
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
	new->str = NULL;
	
	return new;
}

PRODUCT changeProductCode(PRODUCT product, char* str) {
	if (product->str)
		free(product->str);

	product->str = malloc(sizeof(char) * strlen(str) + 1);
	strcpy(product->str, str);

	return product;
}

PRODUCT toProduct(char *str) {
	PRODUCT new;

	new = malloc(sizeof(*new));
	new->str = malloc(sizeof(char) * strlen(str) + 1);

	strcpy(new->str, str);

	return new;
}

PRODUCT cloneProduct(PRODUCT product) {
	PRODUCT new;

	new = malloc(sizeof(*new));
	new->str = malloc(sizeof(char) * strlen(product->str) + 1);

	strcpy(new->str, product->str);

	return new;
}

char* fromProduct(PRODUCT product) {
	char *str = malloc(sizeof(char) * strlen(product->str) + 1);
	strcpy(str, product->str);

	return str;
}

bool isEmptyProduct(PRODUCT p) {
	return (p->str == NULL);
}

void freeProduct(PRODUCT product) {
	if (product->str)
		free(product->str);

	free(product);
}

LIST fillProductSet(PRODUCTCAT productCat, char index) {
	SET set;
	set = fillSet(productCat->cat, index - 'A');

	return toList(set);
}
