#ifndef __PRODUCTS_H__
#define __PRODUCTS_H__

#include "catalog.h"
#include "generic.h"

#define PRODUCT_LENGTH 7

typedef struct product         *PRODUCT;
typedef struct product_catalog *PRODUCTCAT;
typedef struct product_set     *PRODUCTSET;

PRODUCTCAT initProductCat ();
PRODUCTCAT insertProduct  (PRODUCTCAT catalog, PRODUCT product);
void       freeProductCat (PRODUCTCAT catalog);

bool lookUpProduct  (PRODUCTCAT catalog, PRODUCT product);
int  countProducts  (PRODUCTCAT prodCat, char index);

CATALOG prodToCat (PRODUCTCAT prodCat);

PRODUCT newProduct   ();
PRODUCT toProduct    (char* str);
PRODUCT writeProduct (PRODUCT p, char* str);
PRODUCT cloneProduct (PRODUCT p);
char*   fromProduct  (PRODUCT p, char* dest);
void    freeProduct  (PRODUCT p);

bool isProduct (char* str);

PRODUCTSET initProductSet (int n);
PRODUCTSET fillProductSet (PRODUCTCAT cat, PRODUCTSET ps, char index);
void       freeProductSet (PRODUCTSET ps);

PRODUCT getProductByPos   (PRODUCTSET ps, int pos);
int     getProductSetSize (PRODUCTSET ps);

#endif
