#ifndef __PRODUCTS_H__
#define __PRODUCTS_H__

#include "generic.h"
#include "catalog.h"

typedef struct product *PRODUCT; 
typedef struct prodcat *PRODUCTCAT;
typedef struct productSet *PRODUCTSET;

PRODUCTCAT initProductCat(void);
PRODUCTCAT insertProduct(PRODUCTCAT catalog, PRODUCT product);
bool lookUpProduct(PRODUCTCAT catalog, PRODUCT product);
void freeProductCat(PRODUCTCAT catalog);
bool isProduct(char *str);
char* fromProduct(PRODUCT p);
PRODUCT toProduct(char *s);
PRODUCT cloneProduct(PRODUCT p);
CATALOG prodToCat(PRODUCTCAT prodCat);
int countProducts(PRODUCTCAT prodCat, char index);

PRODUCTSET initProductSet(int n); 
PRODUCTSET insertProductSet(PRODUCTSET ps, PRODUCT p);

#endif
