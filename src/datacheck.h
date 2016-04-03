#ifndef __DATACHECK_H__
#define __DATACHECK_H__

#include "catalog.h"
#include "clients.h"
#include "products.h"

#define M_PRODUCTS 0
#define M_CLIENTS 1

CLIENTCAT writeCCat(FILE *file, CLIENTCAT cat, int *sucLn, int *failLn);
PRODUCTCAT writePCat(FILE *file, PRODUCTCAT cat, int *sucLn, int *failLn);
int checkSales (FILE *file, CATALOG clients, CATALOG products, int *sucLn, int *failLn);

void testsValidSales();

#endif
