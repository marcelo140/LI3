#ifndef __PRODUCTS_H__
#define __PRODUCTS_H__

#define true 1
#define false 0

typedef struct product *PRODUCT; 
typedef CATALOG PRODUCTCAT;
typedef char bool;

PRODUCTCAT initProductCat();
PRODUCTCAT insertProduct(PRODUCTCAT catalog, PRODUCT product);
bool lookUpProduct(PRODUCTCAT catalog, PRODUCT product);
void freeProductCat(PRODUCTCAT catalog);
bool isProduct(char *str);
PRODUCT toProduct(char *s);

#endif
