#include <stdlib.h>
#include <string.h>

#include "catalog.h"
#include "clients.h"

#define CATALOG_SIZE 26

#define INDEX(c)             (c->str[0] - 'A')

/* MUST BE IMPROVED */
#define IS_CAPITAL_LETTER(c) (c >= 'A' && c <= 'Z')
#define IS_IN_RANGE(c, i, j) (c >= i   && c <= j)
#define IS_NUMBER(c)         (c >= '0' && c <= '9')

struct client{
	char str[CLIENT_LENGTH];
};

struct client_catalog {
		CATALOG cat;
};

struct client_set {
	CATSET set;
};

CLIENTCAT initClientCat() {
	CLIENTCAT clientCat = malloc(sizeof (*clientCat));

	clientCat->cat = initCatalog(CATALOG_SIZE, NULL, NULL, NULL);

    return clientCat;
}

CLIENTCAT insertClient(CLIENTCAT clientCat, CLIENT client) {
	clientCat->cat = insertCatalog(clientCat->cat, INDEX(client), client->str, NULL);

	return clientCat;
}

void freeClientCat(CLIENTCAT clientCat) {
	freeCatalog(clientCat->cat);
	free(clientCat);
}

bool lookUpClient(CLIENTCAT clientCat, CLIENT client) {
	return lookUpCatalog(clientCat->cat, INDEX(client), client->str);
}

int countClients(CLIENTCAT clientCat, char index) {
	return countPosElems(clientCat->cat, index-'A');
}

CATALOG getClientCat(CLIENTCAT clientCat) {
	return cloneCatalog(clientCat->cat);
}

CLIENT newClient() {
	CLIENT new = malloc(sizeof(struct client));

	new->str[0] = '\0';

	return new;
}

CLIENT writeClient(CLIENT c, char* str) {
	strncpy(c->str, str, CLIENT_LENGTH);
	return c;
}

CLIENT toClient(char* str) {
	CLIENT new = malloc(sizeof (*new));
	
	new = malloc(sizeof(*new));
	strncpy(new->str, str, CLIENT_LENGTH-1);

	return new;
}

CLIENT cloneClient(CLIENT c) {
	CLIENT new;

	new = malloc(sizeof(*new));
	strcpy(new->str, c->str);

	return new;	
}

char* fromClient(CLIENT c, char* dest) {
	strncpy(dest, c->str, CLIENT_LENGTH);
	return dest;
}

bool isEmptyClient(CLIENT c) {
	return (c->str[0] == '\0');
}
 
bool isClient(char *str) {
	return IS_CAPITAL_LETTER(str[0]) &&
           /* Se str[1] é '5', todos os carateres seguintes são '0' */
	       ((str[1] == '5' && str[2] == '0' && str[3] == '0' && str[4] == '0') ||
           /* Caso contrário, todos os carateres seguintes devem estar entre '0' e '9' */
		    (IS_IN_RANGE(str[1], '1', '4') && IS_IN_RANGE(str[2], '0', '9')
                                           && IS_IN_RANGE(str[3], '0', '9')
                                           && IS_IN_RANGE(str[4], '0', '9')));
}

void freeClient(CLIENT client) {
	free(client);
}
 
CLIENTSET initClientSet(int n) {
	CLIENTSET new = malloc (sizeof (*new));
	new->set = initCatSet(n);

	return new;
}

CLIENTSET fillClientSet(CLIENTCAT catProd, CLIENTSET cs, char index) {
	cs->set = fillCatSet(catProd->cat, cs->set, index - 'A');

	return cs;
}

int getClientSetSize(CLIENTSET cs) {
	return getCatSetSize(cs->set);
}

CLIENT getClientByPos(CLIENTSET cs, int pos) {
	char *str = getKeyPos(cs->set, pos);

	if (str == NULL)
		return NULL;
	
	return toClient(str);
}

void freeClientSet(CLIENTSET cs) {
	freeCatSet(cs->set);
	free(cs);	
}
