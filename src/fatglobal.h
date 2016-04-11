#ifndef __FATGLOBAL_H__
#define __FATGLOBAL_H__

#include "sales.h"
#include "products.h"
#include "generic.h"

#define MONTHS 12
#define BRANCHES 3
#define TOTAL 1
#define NP 2

typedef struct faturacao *FATGLOBAL;
typedef struct fatdata   *FATDATA;

FATGLOBAL initFat (PRODUCTCAT p);
FATGLOBAL addFat  (FATGLOBAL fat, SALE s);

FATDATA monthRevenue(FATGLOBAL fat, char *product, int month, int mode);
int monthRange(FATGLOBAL fat, int min, int max, int *quantT, double *billedT);
CATSET* notSold(FATGLOBAL fat, int mode);

double getBilledFat(FATDATA data, int branch, double *billedN, double *billedP);
int getQuantFat(FATDATA data, int branch, int *billedN, int *billedP);

void freeFat(FATGLOBAL fat);
void freeFatData(FATDATA fd);
#endif
