#ifndef __CATALOG_H__
#define __CATALOG_H__

typedef char bool;
typedef struct catalog *CATALOG;

#define true 1
#define false 0

CATALOG initCatalog();
CATALOG insertCatalog(CATALOG c, char *buffer);
bool lookUpCatalog(CATALOG c, char *buffer);
void freeCatalog(CATALOG c);

#endif
