#include <stdlib.h>
#include <string.h>
#include "catalog.h"
#include "avl.h"

#include <stdio.h>

struct catalog{
	AVL *root;
	int size;
};

/**
 * Inicia um novo Catálogo
 * @param n Número de índices do catálogo
 * @return O novo Catálogo
 */
CATALOG initCatalog(int n) {
	CATALOG c = malloc(sizeof (*c));
	int i;
	
	c->root = malloc(sizeof(*c->root) * n);
	c->size = n;

	for (i=0; i < n; i++)
		c->root[i] = initAVL();

	return c;
}

/**
 * Insere conteúdo num dado Catálogo com a respetica Hash
 * @param c Catálogo
 * @param i Índice onde inserir
 * @param s String a inserir
 * @param cntt Conteúdo do catálogo
 * @return Catálogo novo
 */
CATALOG insertCatalog(CATALOG c, int i, char *hash, void *cntt) {
	AVL p = c->root[i];

	c->root[i] = insertAVL(p, hash, cntt);

	return c;
}

/**
 * Atualiza o conteúdo de um elemento de um catálogo caracterizado por uma hash
 * @param c Catálogo
 * @param i Índice onde atualizar
 * @param hash Hash do elemento
 * @param cntt Conteúdo novo
 * @return Catálogo com o conteúdo atualizado
 */
CATALOG updateCatalog(CATALOG c, int i, char *hash, void *cntt) {
	AVL p = c->root[i];
	
	c->root[i] = updateAVL(p, hash, cntt);

	return c;
}

CATALOG cloneCatalog(CATALOG cat, void *(*cloneCntt)(void *cntt)) {
	CATALOG c = malloc(sizeof(*c));
	int i;

	for (i = 0; i < c->size; i++)
		c->root[i] = cloneAVL(cat->root[i], cloneCntt);
	
	return c;
}

/**
 * Devolve o conteúdo de um elemento caracterizado por uma hash
 * @param c Catálogo
 * @param i Índice do conteúdo a devolver
 * @param hash Hash do elemento
 * @return Conteúdo do elemento
 */
void* getCatContent(CATALOG c, int i, char *hash) {
	AVL p = c->root[i];
	
	return getAVLcontent(p, hash); 
}

/**
 * Dado um Catálogo e uma String localiza essa string no catálogo.
 * @param c Catálogo
 * @param i Índice onde procurar
 * @param s String a procurar
 * @return true caso encontre, false caso contrário
 */
bool lookUpCatalog(CATALOG c, int i, char *s) {
	AVL p = c->root[i];

	return lookUpAVL(p, s);
}


int countCatElems(CATALOG c, int i){
	return countNodes(c->root[i]);
}

/**
 * Liberta todo o espaço ocupado pelo catálogo
 * @param c Catálogo a libertar
 * @return void
 */
void freeCatalog(CATALOG c){
	int i;

	if (c){
		for (i=0; i < c->size; i++) freeAVL(c->root[i]);
		free(c);
	}
}
