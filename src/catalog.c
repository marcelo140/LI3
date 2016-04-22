#include <stdlib.h>
#include <string.h>
#include "catalog.h"
#include "avl.h"

struct catalog{
	AVL *root;
	int size;
};

struct member {
	ELEMENT element;
};

CATALOG initCatalog(int n, clone_t clone, free_t free) {
	CATALOG c;
	int i;

	c = malloc(sizeof (*c));
	c->root = malloc(sizeof(*c->root) * n);
	c->size = n;

	for (i=0; i < n; i++)
		c->root[i] = initAVL(NULL, clone, free);

	return c;
}

CATALOG changeCatalogOps (CATALOG cat, clone_t clone, free_t free){
	int i, size = cat->size;
	
	for(i = 0; i < size; i++)
		changeOps(cat->root[i], NULL, clone, free);

	return cat;
}

CATALOG insertCatalog(CATALOG cat, int i, char *hash, void *content) {
	cat->root[i] = insertAVL(cat->root[i], hash, content);

	return cat;
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

MEMBER newMember() {
	MEMBER member = malloc(sizeof(*member));
	member->element = newElement();
	
	return member;
}

void* getCatContent(CATALOG c, int index, char *hash, MEMBER member) {
	ELEMENT elem = NULL;

	if (member)
		elem = member->element;
	
	return getAVLcontent(c->root[index], hash, elem);
}

void updateMember(MEMBER member, void* content) {
	updateElement(member->element, content);
}

void freeMember(MEMBER member) {
	if (member) {
		freeElement(member->element);
		free(member);
	}	
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
