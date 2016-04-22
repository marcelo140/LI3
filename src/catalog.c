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

bool isEmptyCatalog (CATALOG cat) {
	int i;
	bool r = true;

	for (i = 0; r && i < cat->size ; i++) 
		r = isEmptyAVL(cat->root[i]);

	return r;
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

SET fillSet(CATALOG cat, SET set, int index) {
	set = addAVLtoSet(set, cat->root[index]);

	return set;
}

SET fillAllSet(CATALOG cat, SET set) {
	int i, size = cat->size;

	if (size == 0)
		return NULL;

	for(i = 0; i < size; i++)
		set = addAVLtoSet(set, cat->root[i]);

	return set;
}

SET filterCat (CATALOG cat, SET set, condition_t condition, void* arg) {
	int i, size = cat->size;

	for(i = 0; i < size; i++)
		set = filterAVL(cat->root[i], set, condition, arg);

	return set;
}

SET dumpCatalog(CATALOG cat, SET set, void* (*dumper)(void*)) {
	int i, size = cat->size;

	for (i = 0; i < size; i++)
		set = dumpAVL(cat->root[i], set, dumper);

	return set;
}
