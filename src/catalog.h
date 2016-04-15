#ifndef __CATALOG__
#define __CATALOG__

#include "generic.h"

typedef struct catalog      *CATALOG;
typedef struct catalog_set  *CATSET;

typedef void* (*cat_init_t)   ();
typedef void* (*cat_clone_t)  (void*);
typedef bool  (*cat_equals_t) (void*, void*);
typedef void  (*cat_free_t)   (void*);

CATALOG initCatalog (int n,
					 void* (*init)   (), 
                     bool  (*equals) (void*, void*), 
                     void* (*clone)  (void*), 
                     void  (*free)   (void*));

CATALOG changeCatalogOps (CATALOG cat,
					      void* (*init)   (), 
                          bool  (*equals) (void*, void*), 
                          void* (*clone)  (void*), 
                          void  (*free)   (void*));

CATALOG cloneCatalog   (CATALOG cat);

CATALOG insertCatalog  (CATALOG c, int i, char* hash, void* content);
void*   replaceCatalog (CATALOG c, int i, char* hash, void* content);
void*   getCatContent  (CATALOG c, int i, char* hash);

void*   addCatalog     (CATALOG c, int index, char *hash);

bool lookUpCatalog (CATALOG c, int i, char* hash);
int  countPosElems (CATALOG c, int i);
int  countAllElems (CATALOG c);

void freeCatalog (CATALOG c);


CATSET initCatalogSet (int n);
CATSET fillCatalogSet (CATALOG cat, CATSET cs, int i);
CATSET allCatalogSet  (CATALOG cat, CATSET cs);
CATSET contcpy        (CATSET dest, CATSET src, int pos);

CATSET unionCatalogDataSets (CATSET dest, CATSET source);
CATSET diffCatalogDataSets  (CATSET dest, CATSET source);

char* getKeyPos         (CATSET cs, int pos);
void* getContPos        (CATSET cs, int pos);
int   getCatalogSetSize (CATSET cs); 
void  freeCatalogSet    (CATSET cs);

#endif
