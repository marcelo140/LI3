#include <stdlib.h>
#include <string.h>
#include "catalog.h"
#include "avl.h"

struct catalog{
	AVL *root;
	int size;
};

struct catset {
	DATASET set;
};

/**
 * Inicia um novo Catálogo
 * @param n Número de índices do catálogo
 * @return O novo Catálogo
 */
CATALOG initCatalog(int n, void* (*init)(), bool (*equals)(void*, void*), void* (*clone)(void*), void (*free)(void *)) {

	CATALOG c;
	int i;

	c = malloc(sizeof (*c));
	c->root = malloc(sizeof(*c->root) * n);
	c->size = n;

	for (i=0; i < n; i++)
		c->root[i] = initAVL(init, equals, clone, free);

	return c;
}

/**
 * Insere conteúdo num dado Catálogo com a respetica Hash
 * @param c Catálogo
 * @param i Índice onde inserir
 * @param s String a inserir
 * @param content Conteúdo do catálogo
 * @return Catálogo novo
 */
CATALOG insertCatalog(CATALOG c, int i, char *hash, void *content) {
	c->root[i] = insertAVL(c->root[i], hash, content);

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
void* addCatalog(CATALOG c, int i, char *hash) {
	return addAVL(c->root[i], hash);
}

void *replaceCatalog(CATALOG c, int i, char *hash, void *cntt) {
	AVL p = c->root[i];

	return replaceAVL(p, hash, cntt);
}

CATALOG cloneCat(CATALOG cat, void* (*init)(), bool (*equals)(void*, void*), void* (*clone)(void*), void (*free)(void *)) {

	CATALOG c;
	int i;

	c = malloc(sizeof(*c));
	c->root = malloc(sizeof(*c->root) * cat->size);
	c->size = cat->size;

	for (i = 0; i < cat->size; i++)
		c->root[i] = cloneAVL(cat->root[i], init, equals, clone, free);

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

int countAllElems(CATALOG c) {
	int i, size;

	size = 0;
	for(i = 0; i < c->size; i++)
		size += countNodes(c->root[i]);

	return size;
}

/**
 * Liberta todo o espaço ocupado pelo catálogo
 * @param c Catálogo a libertar
 * @return void
 */
void freeCatalog(CATALOG c){
	int i, size;

	if (c){
		size = c->size;
		for (i=0; i < size; i++)
			freeAVL(c->root[i]);

		free(c->root);
		free(c);
	}
}

CATSET initCatalogSet(int n) {
	CATSET cs = malloc(sizeof(struct catset));
	cs->set = initDataSet(n);

	return cs;
}

CATSET fillCatalogSet(CATALOG cat, CATSET cs, int i) {
	cs->set = addDataSet(cs->set, cat->root[i]);
	return cs;
}

CATSET allCatalogSet(CATALOG cat, CATSET cs) {
	int i;

	if (cat->size == 0)
		return NULL;

	for(i = 0; i < cat->size; i++)
		cs->set = addDataSet(cs->set, cat->root[i]);

	return cs;
}

CATSET unionCatalogDataSets(CATSET dest, CATSET source) {
	dest->set = unionDataSets(dest->set, source->set);
	return dest;
}

CATSET diffCatalogDataSets(CATSET dest, CATSET source) {
	dest->set = diffDataSets(dest->set, source->set);
	return dest;
}

CATSET contcpy(CATSET dest, CATSET src, int pos) {
	dest->set = datacpy(dest->set, src->set, pos);
	return dest;
}

char *getKeyPos(CATSET cs, int pos) {
	return getHashPos(cs->set, pos);
}

void *getContPos(CATSET cs, int pos) {
	return getDataPos(cs->set, pos);
}

int getCatalogSetSize(CATSET cs) {
	return getDataSetSize(cs->set);
}

void freeCatalogSet(CATSET cs) {
	freeDataSet(cs->set);
	free(cs);
}
