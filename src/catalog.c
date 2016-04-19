#include <stdlib.h>
#include <string.h>
#include "catalog.h"
#include "avl.h"

struct catalog{
	AVL *root;
	int size;
};

CATALOG initCatalog(int n, init_t init, clone_t clone, free_t free) {
	CATALOG c;
	int i;

	c = malloc(sizeof (*c));
	c->root = malloc(sizeof(*c->root) * n);
	c->size = n;

	for (i=0; i < n; i++)
		c->root[i] = initAVL(init, NULL, clone, free);

	return c;
}

CATALOG changeCatalogOps (CATALOG cat, init_t init, clone_t clone, free_t free){
	int i, size = cat->size;
	
	for(i = 0; i < size; i++)
		changeOps(cat->root[i], init, NULL, clone, free);

	return cat;
}

CATALOG insertCatalog(CATALOG cat, int i, char *hash, void *content) {
	cat->root[i] = insertAVL(cat->root[i], hash, content);

	return cat;
}

void *replaceCatalog(CATALOG cat, int i, char *hash, void *content) {
	return replaceAVL(cat->root[i], hash, content);
}

CATALOG cloneCatalog(CATALOG cat){
	CATALOG c;
	int i, size = cat->size;

	c = malloc(sizeof(*c));
	c->root = malloc(sizeof(*c->root) * size);
	c->size = size;

	for (i = 0; i < size; i++)
		c->root[i] = cloneAVL(cat->root[i]);

	return c;
}

void* getCatContent(CATALOG c, int index, char *hash) {
	return getAVLcontent(c->root[index], hash);
}

void* addCatalog(CATALOG c, int index, char *hash) {
	return addAVL(c->root[index], hash);
}

bool lookUpCatalog(CATALOG cat, int index, char *hash) {
	return lookUpAVL(cat->root[index], hash);
}

int countAllElems(CATALOG cat) {
	int i, size = 0, catSize = cat->size;

	for(i = 0; i < catSize; i++)
		size += countNodes(cat->root[i]);

	return size;
}

int countPosElems(CATALOG c, int index){
	return countNodes(c->root[index]);
}

void freeCatalog(CATALOG cat){
	int i, size;

	if (cat){
		size = cat->size;

		for (i=0; i < size; i++)
			freeAVL(cat->root[i]);

		free(cat->root);
		free(cat);
	}
}

SET fillSet(CATALOG cat, SET s, int index) {
	s = addAVLtoSet(s, cat->root[index]);

	return s;
}

SET fillAllSet(CATALOG cat, SET cs) {
	int i, size = cat->size;

	if (size == 0)
		return cs;

	for(i = 0; i < size; i++)
		cs = addAVLtoSet(cs, cat->root[i]);

	return cs;
}

SET filterCat (CATALOG cat, condition_t condition, void* arg) {
	int i, size = cat->size;
	SET cs = initSet(size);

	for(i = 0; i < size; i++)
		cs = filterAVL(cat->root[i], cs, condition, arg);

	return cs;
}

void separateCat (CATALOG cat, compare_t comp, void* arg, SET set1, SET set2) {
	int i, size = cat->size;

	for(i = 0; i < size; i++)
		separateAVL(cat->root[i], set1, set2, comp, arg);
}

void* dumpDataCat(CATALOG cat, void* data, void* (*dumper)(void*, void*)) {
	int i, size = cat->size;

	for (i = 0; i < size; i++)
		data = dumpDataAVL(cat->root[i], data, dumper);

	return data;
}

void condSeparateCat (CATALOG cat, SET set1, SET set2,
                                   condition_t predicate, void* predicateArg,
                                   compare_t  comparator, void* comparatorArg) {
	int i, size = cat->size;

	for(i = 0; i < size; i++)
		condSeparateAVL(cat->root[i], set1, set2, predicate, predicateArg,
                                                  comparator, comparatorArg);
}
