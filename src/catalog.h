#ifndef __CATALOG_H__
#define __CATALOG_H__

#include "generic.h"

typedef struct catalog *CATALOG;

CATALOG initCatalog(int n);
CATALOG insertCatalog(CATALOG c, int i, char *hash, void *content);
CATALOG updateCatalog(CATALOG c, int i, char *hash, void *content);
/* CATALOG cloneCatalog(CATALOG cat, void *(*cloneCntt)(void *cntt)); */
void* getCatContent(CATALOG c, int i, char *hash);
bool lookUpCatalog(CATALOG c, int i, char *hash);
void freeCatalog(CATALOG c);
int countCatElems(CATALOG c, int i);

#endif
