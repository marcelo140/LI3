#ifndef __PRODUCTS_H__
#define __PRODUCTS_H__

#include "generic.h"
#include "catalog.h"

typedef struct product *PRODUCT; 
typedef CATALOG PRODUCTCAT;

PRODUCTCAT initProductCat(void);
PRODUCTCAT insertProduct(PRODUCTCAT catalog, PRODUCT product);
bool lookUpProduct(PRODUCTCAT catalog, PRODUCT product);
void freeProductCat(PRODUCTCAT catalog);
bool isProduct(char *str);
PRODUCT toProduct(char *s);

#endif
