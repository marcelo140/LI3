#ifndef __CLIENTS_H__
#define __CLIENTS_H__

#include "generic.h"
#include "catalog.h"

typedef struct clientCat *CLIENTCAT;
typedef struct client *CLIENT;

CLIENTCAT initClientCat(void);
CLIENTCAT insertClient(CLIENTCAT catalog, CLIENT client);
bool lookUpClient(CLIENTCAT catalog, CLIENT client);
void freeClientCat(CLIENTCAT catalog);

CLIENT toClient(char *str);
bool isClient(char *str);

#endif
