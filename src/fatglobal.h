#ifndef __FATGLOBAL_H__
#define __FATGLOBAL_H__

#include "sales.h"
#include "products.h"
#include "generic.h"
#include "set.h"
#define MONTHS 12
#define BRANCHES 3
#define SALEMODE 2

#define BY_QUANTITY 0
#define BY_BILLING  1

typedef struct faturacao *FATGLOBAL;

FATGLOBAL initFat (PRODUCTCAT p);

FATGLOBAL addFat  (FATGLOBAL fat, SALE s);

int getProductDataByMonth(FATGLOBAL fat, PRODUCT prod, int month, double billed[][2],
                                                                      int quant[][2]);

double getBilledByMonthRange(FATGLOBAL fat, int initialMonth, int finalMonth);

int getQuantByMonthRange(FATGLOBAL fat, int initialMonth, int finalMonth);

SET getProductsSold(FATGLOBAL fat);

SET getProductsNotSold(FATGLOBAL fat);

SET* getProductsNotSoldByBranch(FATGLOBAL);

SET sortProductGroup(SET pg, int mode);

void freeFat(FATGLOBAL fat);

void freeProductGroup(SET pg);

char* getProductCode(SET pg, int pos);

#endif
