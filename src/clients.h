#ifndef __CLIENTS_H__
#define __CLIENTS_H__

#include "generic.h"
#include "catalog.h"

typedef struct clientCat *CLIENTCAT;
typedef struct client *CLIENT;
typedef struct clientSet *CLIENTSET;

CLIENTCAT initClientCat();
CLIENTCAT insertClient(CLIENTCAT catalog, CLIENT client);
bool lookUpClient(CLIENTCAT catalog, CLIENT client);
void freeClientCat(CLIENTCAT catalog);

CLIENT toClient(char *str);
char *fromClient(CLIENT c);
CLIENT cloneClient(CLIENT c);
void freeClient(CLIENT c);
bool isClient(char *str);

CLIENTSET initCset(int n);
CLIENTSET insertCset(CLIENTSET cs, CLIENT c);
CLIENTSET fillCset(CLIENTCAT catProd, CLIENTSET cs, char index);
CLIENT getCsetData(CLIENTSET cs, int pos);
int csetSize(CLIENTSET cs);

#endif
