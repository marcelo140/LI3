#ifndef __CATALOG_H__
#define __CATALOG_H__

#include "generic.h"

typedef struct catalog *CATALOG;
typedef struct dataSet *KEYSET;

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

KEYSET initKeySet (int n);
KEYSET fillKeySet (CATALOG cat, KEYSET ds, int i);

char *getKeySet    (KEYSET ds, int pos);
int  getKeySetSize (KEYSET ds); 
void freeKeySet    (KEYSET set);

#endif
