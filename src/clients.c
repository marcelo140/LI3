#include <stdlib.h>
#include <string.h>

#include "catalog.h"
#include "clients.h"

#define CATALOG_SIZE 26

#define INDEX(c) (c->str[0] - 'A')

struct client{
	char *str;
};

struct client_catalog {
		CATALOG cat;
};

struct client_set {
	SET set;
};

CLIENTCAT initClientCat() {
	CLIENTCAT clientCat = malloc(sizeof (*clientCat));

	clientCat->cat = initCatalog(CATALOG_SIZE, NULL, NULL);

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

bool isEmptyClientCat (CLIENTCAT clientCat) {
	return isEmptyCatalog(clientCat->cat);	
}

int countClients(CLIENTCAT clientCat, char index) {
	return countPosElems(clientCat->cat, index-'A');
}

CATALOG getClientCat(CLIENTCAT clientCat) {
	return cloneCatalog(clientCat->cat);
}

CLIENT newClient() {
	CLIENT new = malloc(sizeof(struct client));
	new->str = NULL;

	return new;
}

CLIENT changeClientCode(CLIENT c, char* str) {
	if (c->str)
		free(c->str);

	c->str = malloc(sizeof(char) * strlen(str) + 1);
	strcpy(c->str, str);

	return c;
}

CLIENT toClient(char* str) {
	CLIENT new;

	new  = malloc(sizeof (*new));
	new->str = malloc(sizeof(char) * strlen(str) + 1);
	
	strcpy(new->str, str);

	return new;
}

CLIENT cloneClient(CLIENT c) {
	CLIENT new;

	new = malloc(sizeof(*new));
	new->str = malloc(sizeof(char) * strlen(c->str) + 1);

	strcpy(new->str, c->str);

	return new;	
}

char* fromClient(CLIENT c) {
	char *str = malloc(sizeof(char) * strlen(c->str) + 1);
	strcpy(str, c->str);

	return str;
}

bool isEmptyClient(CLIENT c) {
	return (c->str == NULL);
}

void freeClient(CLIENT client) {
	if (client->str)
		free(client->str);

	free(client);
}
 
SET fillClientSet(CLIENTCAT catProd, char index) {
	SET set = initSet(countAllElems(catProd->cat), NULL);

	set = fillSet(catProd->cat, set, index - 'A');

	return set;
}
