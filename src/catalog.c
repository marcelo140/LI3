#include <stdlib.h>
#include <string.h>
#include "catalog.h"
#include "avl.h"

#define ARRAY_SIZE 26

struct catalog{
	AVL root[ARRAY_SIZE];
};

/**
 * Inicia um novo Catálogo
 * @return O novo Catálogo
 */
CATALOG initCatalog() {
	CATALOG c = malloc(sizeof (*c));
	int i;

	for (i=0; i<ARRAY_SIZE; i++)
		c->root[i] = initAVL();

	return c;
}

/**
 * Insere conteúdo num dado Catálogo com a respetica Hash
 * @param c Catálogo
 * @param s String a inserir
 * @param cntt Conteúdo do catálogo
 * @return Catálogo novo
 */
CATALOG insertCatalog(CATALOG c, char *hash, void *cntt) {
	int pos = hash[0] - 'A';
	AVL p = c->root[pos];

	c->root[pos] = insertAVL(p, hash, cntt);

	return c;
}

/**
 * Atualiza o conteúdo de um elemento de um catálogo caracterizado por uma hash
 * @param c Catálogo
 * @param hash Hash do elemento
 * @param cntt Conteúdo novo
 * @return Catálogo com o conteúdo atualizado
 */
CATALOG updateCatalog(CATALOG c, char *hash, void *cntt) {
	int pos = hash[0] - 'A';
	AVL p = c->root[pos];
	
	c->root[pos] = updateAVL(p, hash, cntt);

	return c;
}

CATALOG cloneCatalog(CATALOG cat, void *(*cloneCntt)(void *cntt)) {
	CATALOG c = malloc(sizeof(*c));
	int i;

	for (i = 0; i < ARRAY_SIZE; i++)
		c->root[i] = cloneAVL(cat->root[i], cloneCntt);

	return c;
}

/**
 * Devolve o conteúdo de um elemento caracterizado por uma hash
 * @param c Catálogo
 * @param hash Hash do elemento
 * @return Conteúdo do elemento
 */
void* getCatontent(CATALOG c, char *hash) {
	int pos = hash[0] - 'A';
	AVL p = c->root[pos];
	
	return getAVLcontent(p, hash); 
}

/**
 * Dado um Catálogo e uma String localiza essa string no catálogo.
 * @param c Catálogo
 * @param s String a procurar
 * @return true caso encontre, false caso contrário
 */
bool lookUpCatalog(CATALOG c, char *s) {
	int pos = s[0] - 'A';
	AVL p = c->root[pos];

	return lookUpAVL(p, s);
}

/**
 * Liberta todo o espaço ocupado pelo catálogo
 * @param c Catálogo a libertar
 * @return void
 */
void freeCatalog(CATALOG c){
	int i;

	if (c){
		for (i=0; i<ARRAY_SIZE; i++) freeAVL(c->root[i]);
		free(c);
	}
}
