#ifndef __FATGLOBAL_H__
#define __FATGLOBAL_H__

#include "sales.h"
#include "products.h"
#include "generic.h"

#define MONTHS 12
#define TOTAL 1
#define BRANCHES 3
#define NP 2

typedef struct faturacao *FATGLOBAL;
typedef struct fatdata   *FATDATA;

FATGLOBAL initFat (PRODUCTCAT p);
FATGLOBAL addFat  (FATGLOBAL fat, SALE s);

FATDATA monthRevenue(FATGLOBAL fat, char *product, int month, int mode);
CATSET* notSold(FATGLOBAL fat, int mode);

void freeFat(FATGLOBAL fat);

#endif
