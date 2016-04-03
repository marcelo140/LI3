#include <stdlib.h>
#include <string.h>
#include "catalog.h"
#include "node.h"

#define ARRAY_SIZE 26

struct catalog{
	NODE root[ARRAY_SIZE];
};

/**
 * Inicia um novo Catálogo
 * @return O novo Catálogo
 */
CATALOG initCatalog() {
	CATALOG c = malloc(sizeof (*c));
	int i;

	for (i=0; i<ARRAY_SIZE; i++)
		c->root[i] = initNode();

	return c;
}

/**
 * Insere uma dada string num dado Catálogo
 * @param c Catálogo
 * @param s String a inserir
 * @return Catálogo novo
 */
CATALOG insertCatalog(CATALOG c, char *s) {
	int pos = s[0] - 'A';
	NODE p = c->root[pos];

	c->root[pos] = insertNode(p, s);

	return c;
}

/**
 * Dado um Catálogo e uma String localiza essa string no catálogo.
 * @param c Catálogo
 * @param s String a procurar
 * @return true caso encontre, false caso contrário
 */
bool lookUpCatalog(CATALOG c, char *s) {
	int pos = s[0] - 'A';
	NODE p = c->root[pos];

	return lookUpNode(p, s);
}

/**
 * Liberta todo o espaço ocupado pelo catálogo 
 * @param c Catálogo a libertar
 * @return void
 */
void freeCatalog(CATALOG c){
	int i;

	if (c){
		for (i=0; i<ARRAY_SIZE; i++) freeNode(c->root[i]);
		free(c);
	}
}
