#ifndef __CLIENTS_H__
#define __CLIENTS_H__

#include "catalog.h"

typedef char bool;

typedef CATALOG CLIENTCAT;
typedef struct client *CLIENT;

CLIENTCAT initClientCat();
CLIENTCAT insertClient(CLIENTCAT catalog, CLIENT client);
bool lookUpClient(CLIENTCAT catalog, CLIENT client);
void freeClientCat(CLIENTCAT catalog);

CLIENT toClient(char *str);
bool isClient(char *str);
#endif
