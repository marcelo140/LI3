#ifndef __FATGLOBAL__
#define __FATGLOBAL__

#include "generic.h"
#include "sales.h"
#include "products.h"
#include "set.h"

typedef struct faturacao *FATGLOBAL;
typedef struct product_fat *PRODUCTFAT;

#define MONTHS 12
#define BRANCHES 3
#define SALEMODE 2

FATGLOBAL initFat ();

FATGLOBAL initFat (PRODUCTCAT p);

FATGLOBAL addSaleToFat  (FATGLOBAL fat, SALE s);

PRODUCTFAT getProductDataByMonth(FATGLOBAL fat, PRODUCT p, int month);

double getBilledByMonthRange(FATGLOBAL fat, int initialMonth, int finalMonth);

int getQuantByMonthRange(FATGLOBAL fat, int initialMonth, int finalMonth);

LIST getProductsNotSold(FATGLOBAL fat);

LIST* getProductsNotSoldByBranch(FATGLOBAL);

int getProductsFatQuant(PRODUCTFAT, int branch, int* normal, int* promo);

double getProductFatBilled(PRODUCTFAT, int branch, double* normal, double* promo);

void freeFat(FATGLOBAL fat);

void freeProductFat(PRODUCTFAT pf);

#endif
