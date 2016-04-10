#ifndef __FATGLOBAL_H__
#define __FATGLOBAL_H__

#include "sales.h"
#include "products.h"
#include "generic.h"

#define MONTHS 12
#define BRANCHES 3
#define NP 2

typedef struct faturacao *FATGLOBAL;

FATGLOBAL initFat (PRODUCTCAT p);
FATGLOBAL addFat  (FATGLOBAL fat, SALE s);

void freeFat(FATGLOBAL fat);

#endif
