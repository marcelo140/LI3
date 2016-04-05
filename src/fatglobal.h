#ifndef __FATGLOBAL_H__
#define __FATGLOBAL_H__

#include "generic.h"

#define MONTHS 12
#define BRANCHES 3
#define NP 2

#define N 0
#define P 1



typedef struct faturacao *FATGLOBAL;

FATGLOBAL initFat(void);
FATGLOBAL addFat(FATGLOBAL fat, PRODUCT p, int month, int branch, int type, double f)
bool isEmptyFat(FATGLOBAL f);


#endif
