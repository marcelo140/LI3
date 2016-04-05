#ifndef __CATALOG_H__
#define __CATALOG_H__

#include "generic.h"

typedef struct catalog *CATALOG;

CATALOG initCatalog(void);
CATALOG insertCatalog(CATALOG c, char *hash, void *content);
CATALOG updateCatalog(CATALOG c, char *hash, void *content);
void* getCatalogContent(CATALOG c, char *hash);
bool lookUpCatalog(CATALOG c, char *hash);
void freeCatalog(CATALOG c);

#endif
