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
int countProducts(PRODUCTCAT prodCat, char index);

bool isProduct(char *str);
char* fromProduct(PRODUCT p);
PRODUCT toProduct(char *s);
PRODUCT cloneProduct(PRODUCT p);
void freeProduct(PRODUCT p);

CATALOG prodToCat(PRODUCTCAT prodCat);
PRODUCTSET initProductSet(int n); 
PRODUCTSET insertProductSet(PRODUCTSET ps, PRODUCT p);
PRODUCTSET fillProductSet(PRODUCTCAT cat, PRODUCTSET ps, char index);
PRODUCT getProduct(PRODUCTSET ps, int pos);
int getProductSetSize(PRODUCTSET ps);

#endif
