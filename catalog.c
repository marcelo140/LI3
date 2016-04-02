#include <stdlib.h>
#include <string.h>
#include "catalog.h"


CATALOG initCatalog () {
	CATALOG c = malloc(sizeof (*c));
	int i;

	for (i=0; i<26; i++) c->root[i] = initNode();

	return c;
}

CATALOG insertCatalog(CATALOG c, char *buffer) {

	int pos = buffer[0] - 'A';
	NODE p = c->root[pos];

	c->root[pos] = insertNode(p, buffer);

	return c;
}

/*Dado um catálogo e uma String localiza essa string no catálogo.*/
int lookUpCatalog(CATALOG c, char *buffer) {
	int pos = buffer[0] - 'A';
	NODE p = c->root[pos];

	return lookUpNode(p, buffer);
}

/* Liberta todo o espaço ocupado pelo catálogo */
void freeCatalog(CATALOG c){

	int i;

	if (!c){
		for (i=0; i<26; i++) freeNode(c->root[i]);
		free(c);
	}
}
