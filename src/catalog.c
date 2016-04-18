#include <stdlib.h>
#include <string.h>
#include "catalog.h"
#include "avl.h"

struct catalog{
	AVL *root;
	int size;
};

struct catalog_set {
	DATASET set;
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

CATSET initCatSet(int n) {
	CATSET cs = malloc(sizeof(*cs));
	cs->set = initDataSet(n);

	return cs;
}

CATSET fillCatSet(CATALOG cat, CATSET cs, int index) {
	cs->set = addDataSet(cs->set, cat->root[index]);

	return cs;
}

CATSET fillAllCatSet(CATALOG cat, CATSET cs) {
	int i, size = cat->size;

	if (size == 0)
		return NULL;

	for(i = 0; i < size; i++)
		cs->set = addDataSet(cs->set, cat->root[i]);

	return cs;
}

CATSET contcpy(CATSET dest, CATSET src, int pos) {
	dest->set = datacpy(dest->set, src->set, pos);

	return dest;
}

CATSET filterCat (CATALOG cat, condition_t condition, void* arg) {
	int i, size = cat->size;
	CATSET cs = initCatSet(size);

	for(i = 0; i < size; i++)
		cs->set = filterAVL(cat->root[i], cs->set, condition, arg);

	return cs;
}

void separateCat (CATALOG cat, compare_t comp, void* arg, CATSET set1, CATSET set2) {
	int i, size = cat->size;

	for(i = 0; i < size; i++)
		separateAVL(cat->root[i], set1->set, set2->set, comp, arg);
}

void* dumpDataCat(CATALOG cat, void* data, void* (*dumper)(void*, void*)) {
	int i, size = cat->size;

	for (i = 0; i < size; i++)
		data = dumpDataAVL(cat->root[i], data, dumper);

	return data;
}

void condSeparateCat (CATALOG cat, CATSET set1, CATSET set2,
                                   condition_t predicate, void* predicateArg,
                                   compare_t  comparator, void* comparatorArg) {
	int i, size = cat->size;

	for(i = 0; i < size; i++)
		condSeparateAVL(cat->root[i], set1->set, set2->set, predicate, predicateArg,
                                                            comparator, comparatorArg);
}

void sortCatSet(CATSET cs, compare_t comp) {
	sortDataSet(cs->set, 0, getCatSetSize(cs)-1, comp);
}

CATSET concatCatSet(CATSET set1, CATSET set2) {
	set1->set = concatDataSet(set1->set, set2->set);

	return set1;	
}

CATSET unionCatalogDataSets(CATSET dest, CATSET source) {
	dest->set = unionDataSets(dest->set, source->set);

	return dest;
}

CATSET diffCatalogDataSets(CATSET dest, CATSET source) {
	dest->set = diffDataSets(dest->set, source->set);

	return dest;
}

CATSET intersectCatSets(CATSET set1, CATSET set2) {
	CATSET new = malloc(sizeof(*new));

	new->set = intersectDataSet(set1->set, set2->set);
	return new;
}

void *getContPos(CATSET cs, int pos) {
	return getDataPos(cs->set, pos);
}

char *getKeyPos(CATSET cs, int pos){
	if (!cs || !cs->set) 
		return NULL;

	return getHashPos(cs->set, pos);
}

int getCatSetSize(CATSET cs) {
	return getDataSetSize(cs->set);
}

void freeCatSet(CATSET cs) {
	freeDataSet(cs->set);
	free(cs);
}
