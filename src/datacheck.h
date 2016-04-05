#ifndef __DATACHECK_H__
#define __DATACHECK_H__

#include <stdio.h>

#include "generic.h"
#include "clients.h"
#include "products.h"

#define M_PRODUCTS 0
#define M_CLIENTS 1

PRODUCTCAT writePCat(FILE *file, PRODUCTCAT cat, int *num);
CLIENTCAT writeCCat(FILE *file, CLIENTCAT cat, int *num);

int checkSales (FILE *file, PRODUCTCAT products, CLIENTCAT clients, int *sucLn, int *failLn);
void testsValidSales(void);

#endif
