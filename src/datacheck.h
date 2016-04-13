#ifndef __DATACHECK_H__
#define __DATACHECK_H__

#include <stdio.h>

#include "generic.h"
#include "fatglobal.h"
#include "clients.h"
#include "products.h"

int loadSales    (FILE *file,
                  FATGLOBAL fat, 
                  PRODUCTCAT products, 
                  CLIENTCAT clients, 
                  int *failed);

int loadProducts (FILE *file, PRODUCTCAT cat);
int loadClients  (FILE *file, CLIENTCAT cat);

#endif
