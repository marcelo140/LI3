#ifndef __PRODUCTS_H__
#define __PRODUCTS_H__

#include "generic.h"
#include "catalog.h"

typedef struct product *PRODUCT;
typedef struct prodcat *PRODUCTCAT;
typedef struct prodset *PRODUCTSET;

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

PRODUCTSET initPSet(int n);
PRODUCTSET fillPSet(PRODUCTCAT cat, PRODUCTSET ps, char index);
PRODUCT getPSetData(PRODUCTSET ps, int pos);
int getPSetSize(PRODUCTSET ps);
void freePSet(PRODUCTSET ps);

#endif
