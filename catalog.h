#ifndef __CATALOG_H__
#define __CATALOG_H__

#include "node.h"

typedef struct avl *CATALOG;

#define BUFFER_SIZE 10

CATALOG initCatalog();
CATALOG insertCatalog(CATALOG c, char *buffer);
int lookUpCatalog(CATALOG c, char *buffer);
void freeCatalog(CATALOG c);

#endif
