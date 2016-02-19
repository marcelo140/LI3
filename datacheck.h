#ifndef __DATACHECK_H__
#define __DATACHECK_H__

#define M_PRODUCTS 0
#define M_CLIENTS 1
#define M_SALES 2

int checkFile(FILE *file, int mode, int *sucLn, int *failLn);

#endif
