#ifndef __CLIENTS_H__
#define __CLIENTS_H__

#include "generic.h"
#include "catalog.h"

#define CLIENT_LENGTH 6

typedef struct client         *CLIENT;
typedef struct client_catalog *CLIENTCAT;
typedef struct client_set     *CLIENTSET;

CLIENTCAT initClientCat ();
CLIENTCAT insertClient  (CLIENTCAT catalog, CLIENT client);
void      freeClientCat (CLIENTCAT catalog);

bool lookUpClient (CLIENTCAT catalog, CLIENT client);

CLIENT newClient   ();
CLIENT toClient    (char* str);
CLIENT writeClient (CLIENT c, char* str);
CLIENT cloneClient (CLIENT c);
char*  fromClient  (CLIENT c, char* dest);
void   freeClient  (CLIENT c);

bool isClient (char* str);

CLIENTSET initClientSet (int n);
CLIENTSET fillClientSet (CLIENTCAT clientCat, CLIENTSET cs, char index);
void      freeClientSet (CLIENTSET cs);

CLIENT getClientByPos (CLIENTSET cs, int pos);
int    clientSetSize  (CLIENTSET cs);

#endif
