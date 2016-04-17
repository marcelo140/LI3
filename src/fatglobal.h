#ifndef __FATGLOBAL_H__
#define __FATGLOBAL_H__

#include "sales.h"
#include "products.h"
#include "generic.h"

#define MONTHS 12
#define BRANCHES 3
#define SALEMODE 2

#define BY_QUANTITY 0
#define BY_BILLING  1

typedef struct faturacao *FATGLOBAL;
typedef struct product_group *PRODUCTGROUP;
typedef char SORT_MODE;

FATGLOBAL initFat (PRODUCTCAT p);

FATGLOBAL addFat  (FATGLOBAL fat, SALE s);

int getProductDataByMonth(FATGLOBAL fat, PRODUCT prod, int month, double billed[][2],
                                                                      int quant[][2]);

double getBilledByMonthRange(FATGLOBAL fat, int initialMonth, int finalMonth);

int getQuantByMonthRange(FATGLOBAL fat, int initialMonth, int finalMonth);

PRODUCTGROUP getProductsSold(FATGLOBAL fat);

PRODUCTGROUP getProductsNotSold(FATGLOBAL fat);

PRODUCTGROUP* getProductsNotSoldByBranch(FATGLOBAL);

PRODUCTGROUP sortProductGroup(PRODUCTGROUP pg, SORT_MODE mode);

void freeFat(FATGLOBAL fat);

void freeProductGroup(PRODUCTGROUP pg);

char* getProductCode(PRODUCTGROUP pg, int pos);

#endif
