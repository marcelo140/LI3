#include <stdlib.h>
#include <string.h>

#include "set.h"

#define HASH(s,i) s->list[i]->hash
#define CONTENT(s,i) s->list[i]->content
#define SIZE(s) s->size

struct element {
	char* hash;
	void* content;
};

struct set {
	ELEMENT* list;
	int size;
	int capacity;
};

struct list {
	struct set* list;
};

static ELEMENT newElement      (char* hash, void* content);
static void    swapData        (SET set, int i, int j);
static int     partitionByName (SET set, int begin, int end);
static int     partition       (SET set, int begin, int end, compare_t comparator);
static void    quicksortByName (SET set, int begin, int end);
static void    quicksort       (SET set, int begin, int end, compare_t comparator);

LIST toList(SET s) {
	LIST new = malloc(sizeof(*new));
	new->list = s;
	
	return new;
}

char* getListElement(LIST l, int pos) {
	if (pos < 0 || pos >= l->list->size)
		return NULL;

	return l->list->list[pos]->hash;
}

SET initSet(int capacity) {
	SET new = malloc(sizeof(*new));

	new->list = malloc(sizeof(ELEMENT) * capacity);
	new->size = 0;
	new->capacity = capacity;

	return new;
}

SET datacpy(SET dest, SET src, int pos) {
	insertElement(dest, HASH(src,pos), CONTENT(src,pos));

	return dest;
}

SET insertElement(SET s, char* hash, void* content) {
	ELEMENT new = newElement(hash, content);
	int size = s->size;

	if (size == s->capacity) {
		s->capacity *= 2;
		s->list = realloc(s->list, s->capacity * sizeof(ELEMENT));
	}

	s->list[size] = new;
	s->size++;

	return s;
}

char* getSetHash(SET s, int pos) {
	char* str = malloc(sizeof(char) * HASH_SIZE);

	if (pos < 0 || pos >= s->size)
		return NULL;

	strcpy(str, s->list[pos]->hash);

	return str;
}

void* getSetData(SET s, int pos) {
	if (pos < 0 || pos >= s->size)
		return NULL;

	return s->list[pos]->content;
}

int getSetSize(SET s) {
	return s->size;
}

void sortSetByName(SET list) {
	quicksortByName(list, 0, list->size-1);
}

void sortSet(SET list, compare_t comparator) {
	quicksort(list, 0, list->size-1, comparator);
}

SET unionSets(SET s1, SET s2) {
	SET new = initSet(100);
	int res, sizeS2 = 0, sizeS1 = 0, maxSizeS2 = s2->size, maxSizeS1 = s1->size;
	
	while(sizeS2 < maxSizeS2 && sizeS1 < maxSizeS1){
		res = strcmp(HASH(s1,sizeS1), HASH(s2,sizeS2));

		if (res < 0) {
			new = insertElement(new, HASH(s1, sizeS1), CONTENT(s1, sizeS1));
			sizeS1++;
		}else if (res > 0){
			new = insertElement(new, HASH(s2, sizeS2), CONTENT(s2, sizeS2));
			sizeS2++;
		}else{
			new = insertElement(new, HASH(s2, sizeS2), CONTENT(s2, sizeS2));
			sizeS2++;	
			sizeS1++;
		}
	}

	while(sizeS1 < maxSizeS1){
		new = insertElement(new, HASH(s1,sizeS1), CONTENT(s1,sizeS1));
		sizeS1++;
	}
	
	while(sizeS2 < maxSizeS2){
		new = insertElement(new, HASH(s2,sizeS2), CONTENT(s2,sizeS2));
		sizeS2++;
	}

	return new;
}

SET diffDataSets(SET s1, SET s2) {
	SET new = initSet(100);
	int res, sizeS1 = 0, sizeS2 = 0, maxSizeS1 = s1->size, maxSizeS2 = s2->size;

	while(sizeS1 < maxSizeS1 && sizeS2 < maxSizeS2){
		res = strcmp(HASH(s1,sizeS1), HASH(s2, sizeS2));

		if (res < 0) {
			new = insertElement(new, HASH(s1,sizeS1), CONTENT(s1,sizeS1));
			sizeS1++;
		}else if (res > 0){
			new = insertElement(new, HASH(s2,sizeS2), CONTENT(s2,sizeS2));
			sizeS2++;
		}else{
			sizeS1++;
			sizeS2++;
		}
	}

	while(sizeS1 < maxSizeS1){
		new = insertElement(new, HASH(s1,sizeS1), CONTENT(s1,sizeS1));
		sizeS1++;
	}
	
	while(sizeS2 < maxSizeS2){
		new = insertElement(new, HASH(s2,sizeS2), CONTENT(s2,sizeS2));
		sizeS2++;
	}

	return new;
}

SET intersectDataSet(SET s1, SET s2) {
	SET new;
	int i, j, size;

	i = j = 0;
	size = (SIZE(s1) < SIZE(s2)) ? SIZE(s1) : SIZE(s2);
	new = initSet(size);

	while(i < SIZE(s1) && j < SIZE(s1)) {
		if (HASH(s1,i) < HASH(s2,j)) 
			i++;	
		else if (HASH(s1,i) > HASH(s2,j)) 
			 j++;
		else {
			new = insertElement(new, HASH(s1,i), CONTENT(s1,i));
			i++;
			j++;
		}
	}

	return new;
}

void freeList(LIST l) {
	if (l) {
		freeSet(l->list);
		free(l);
	}
}

void freeSet(SET s) {
	if (s) {
		free(s->list);
		free(s);
	}
}

static ELEMENT newElement(char* hash, void* content) {
	ELEMENT new = malloc(sizeof(*new));

	new->hash = malloc(sizeof(char) * HASH_SIZE);
	strncpy(new->hash, hash, HASH_SIZE);
	new->content = content;

	return new;
}

static void swapData(SET set, int i, int j) {
	ELEMENT tmp = set->list[i];
	set->list[i] = set->list[j];
	set->list[j] = tmp;
}

static int partitionByName(SET set, int begin, int end) {
	char* pivot = HASH(set, end);
	int i, lim = begin-1;

	for(i = begin; i < end; i++) {
		if (strcmp(HASH(set,i), pivot) <= 0){
			lim++;
			swapData(set, lim, i);
		}
	}

	swapData(set, lim+1, end);
	return lim+1;
}

static int partition(SET set, int begin, int end, compare_t comparator) {
	char* pivot = CONTENT(set, end);
	int i, lim = begin-1;

	for(i = begin; i < end; i++) {
		if (comparator(CONTENT(set,i), pivot) <= 0){
			lim++;
			swapData(set, lim, i);
		}
	}

	swapData(set, lim+1, end);
	return lim+1;
}

static void quicksort(SET set, int begin, int end, compare_t comparator) {
	int lim;

	if (begin < end) {
		lim = partition(set, begin, end, comparator);
		
		quicksort(set, begin, lim-1, comparator);
		quicksort(set, lim+1, end, comparator);
	}	
}

static void quicksortByName(SET set, int begin, int end) {
	int lim;

	if (begin < end) {
		lim = partitionByName(set, begin, end);
		
		quicksortByName(set, begin, lim-1);
		quicksortByName(set, lim+1, end);
	}
}
