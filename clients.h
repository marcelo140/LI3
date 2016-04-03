#ifndef __CLIENTS_H__
#define __CLIENTS_H__

typedef char bool;

typedef CATALOG CLIENTCAT;
typedef struct client *CLIENT;

#define true 1
#define false 0

CLIENTCAT initClientCat();
CLIENTCAT insertClient(CLIENTCAT catalog, CLIENT client);
bool lookUpClient(CLIENTCAT catalog, CLIENT client);
void freeClientCat(CLIENTCAT catalog);

CLIENT toClient(char *str);
bool isClient(char *str);
#endif
