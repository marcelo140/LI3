#ifndef __FATGLOBAL_H__
#define __FATGLOBAL_H__

#include "sales.h"
#include "products.h"
#include "generic.h"

#define MONTHS 12
#define BRANCHES 3
#define NP 2

typedef struct faturacao *FATGLOBAL;

FATGLOBAL initFat(void);
FATGLOBAL addFat(FATGLOBAL fat, SALE sale);
FATGLOBAL fillFat(PRODUCTCAT p);
bool isEmptyFat(FATGLOBAL f);


#endif
