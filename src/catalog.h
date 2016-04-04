#ifndef __CATALOG_H__
#define __CATALOG_H__

#include "generic.h"

typedef struct catalog *CATALOG;

CATALOG initCatalog(void);
CATALOG insertCatalog(CATALOG c, char *buffer);
bool lookUpCatalog(CATALOG c, char *buffer);
void freeCatalog(CATALOG c);

#endif
