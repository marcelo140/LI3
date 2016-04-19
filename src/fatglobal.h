#ifndef __FATGLOBAL__
#define __FATGLOBAL__

#include "generic.h"
#include "sales.h"
#include "products.h"
#include "set.h"

typedef struct faturacao *FATGLOBAL;
typedef struct product_group *PRODUCTGROUP;

#define MONTHS 12
#define BRANCHES 3
#define SALEMODE 2


#define BY_QUANTITY 0
#define BY_BILLING  1


FATGLOBAL initFat ();

FATGLOBAL fillFat (FATGLOBAL fat, PRODUCTCAT p);

FATGLOBAL addSaleToFat  (FATGLOBAL fat, SALE s);

int getProductDataByMonth(FATGLOBAL fat, PRODUCT p, int month, double b[][2], int q[][2]);

double getBilledByMonthRange(FATGLOBAL fat, int initialMonth, int finalMonth);

int getQuantByMonthRange(FATGLOBAL fat, int initialMonth, int finalMonth);

SET getProductsNotSold(FATGLOBAL fat);

SET* getProductsNotSoldByBranch(FATGLOBAL);

void freeFat(FATGLOBAL fat);

#endif
