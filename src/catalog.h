#ifndef __CATALOG_H__
#define __CATALOG_H__

#include "generic.h"

typedef struct catalog *CATALOG;
typedef struct catset *CATSET;

CATALOG initCatalog (int n,
					 void* (*init)   (), 
                     void* (*join)   (void*, void *), 
                     bool  (*equals) (void*, void*), 
                     void* (*clone)  (void*), 
                     void  (*free)   (void*));

CATALOG cloneCat    (CATALOG cat,
					 void* (*init)   (), 
                     void* (*join)   (void*, void*),
                     bool  (*equals) (void*, void*),
                     void* (*clone)  (void*),
                     void  (*free)   (void*));

CATALOG insertCatalog (CATALOG c, int i, char *hash, void *content);
CATALOG updateCatalog (CATALOG c, int i, char *hash, void *content);

void *replaceCatalog (CATALOG c, int i, char *hash, void *content);
void *getCatContent  (CATALOG c, int i, char *hash);
void freeCatalog     (CATALOG c);

bool lookUpCatalog (CATALOG c, int i, char *hash);
int  countCatElems (CATALOG c, int i);
int  countAllElems  (CATALOG c);

CATSET initCatSet (int n);
CATSET fillCatSet (CATALOG cat, CATSET cs, int i);
CATSET allCatSet  (CATALOG cat, CATSET cs);
CATSET contcpy    (CATSET dest, CATSET src, int pos);

char *getKeyPos    (CATSET cs, int pos);
void *getContPos   (CATSET cs, int pos);
int  getCatSetSize (CATSET cs); 
void freeCatSet    (CATSET cs);

#endif
