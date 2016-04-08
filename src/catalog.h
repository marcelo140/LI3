#ifndef __CATALOG_H__
#define __CATALOG_H__

#include "generic.h"

typedef struct catalog *CATALOG;
typedef struct dataSet *DATASET;

CATALOG initCatalog(int n);
CATALOG insertCatalog(CATALOG c, int i, char *hash, void *content);
CATALOG updateCatalog(CATALOG c, int i, char *hash, void *content);
/* CATALOG cloneCatalog(CATALOG cat, void *(*cloneCntt)(void *cntt)); */
void* getCatContent(CATALOG c, int i, char *hash);
bool lookUpCatalog(CATALOG c, int i, char *hash);
void freeCatalog(CATALOG c);
int countCatElems(CATALOG c, int i);

DATASET initDataSet(int n);
DATASET insertDataSet(DATASET ds, char *data);
DATASET fillDataSet(CATALOG cat, DATASET ds, int i);
char *getDataSet(DATASET ds, int pos);
int getDataSetSize(DATASET ds); 

#endif
