#ifndef __DATACHECK_H__
#define __DATACHECK_H__

#include "avl.h"

#define M_PRODUCTS 0
#define M_CLIENTS 1

void writeCatalog (FILE *file, CATALOG cat, int mode, int *sucLn, int *failLn);
int checkSales (FILE *file, CATALOG clients, CATALOG products, int *sucLn, int *failLn);


#endif
