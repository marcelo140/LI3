#ifndef __DATACHECK_H__
#define __DATACHECK_H__

#include "mockArray.h"

#define M_PRODUCTS 0
#define M_CLIENTS 1
#define M_SALES 2

int checkFile (FILE *file, CATALOG cat1, CATALOG cat2, int mode, int *sucLn, int *failLn);

#endif
