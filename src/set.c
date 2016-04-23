#include <stdlib.h>
#include <string.h>

#include "set.h"

#define HASH(s,i) s->list[i]->hash
#define CONTENT(s,i) s->list[i]->content
#define SIZE(s) s->size

typedef struct element {
	char* hash;
	void* content;
}*ELEMENT;

struct set {
	ELEMENT* list;
	int size;
	int capacity;
	free_t free;
};

static ELEMENT newElement (char* hash, void* content);

static int partitionByName (SET set, int begin, int end);
static void quicksortByName (SET set, int begin, int end);

static void quicksort (SET set, int begin, int end, compare_t comparator, void* arg);
static int partition (SET set, int begin, int end, compare_t comparator, void* arg);

static void swapData (SET set, int i, int j);


SET initSet(int capacity, free_t free) {
	SET new = malloc(sizeof(*new));

	new->list = malloc(sizeof(ELEMENT) * capacity);
	new->size = 0;
	new->capacity = capacity;
	new->free = free;

	return new;
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

SET datacpy(SET dest, SET src, int pos) {
	insertElement(dest, HASH(src,pos), CONTENT(src,pos));

	return dest;
}

char* getSetHash(SET s, int pos) {
	char* str;

	if (pos < 0 || pos >= s->size)
		return NULL;
	
	str = malloc(sizeof(char) * strlen(HASH(s, pos)) + 1);
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

void sortSet(SET list, compare_t comparator, void* arg) {
	quicksort(list, 0, list->size-1, comparator, arg);
}

SET unionSets(SET s1, SET s2) {
	SET new;
	int res, pos2, pos1, size2, size1;
	
	pos2 = pos1 = 0;
	size1 = s1->size;
	size2 = s2->size;
	
	if (size1 > size2)
		new = initSet(size1, s1->free);
	else
		new = initSet(size2, s2->free);

	while(pos2 < size2 && pos1 < size1){
		res = strcmp(HASH(s1,pos1), HASH(s2,pos2));

		if (res < 0) {
			new = insertElement(new, HASH(s1, pos1), CONTENT(s1, pos1));
			pos1++;
		}else if (res > 0){
			new = insertElement(new, HASH(s2, pos2), CONTENT(s2, pos2));
			pos2++;
		}else{
			new = insertElement(new, HASH(s2, pos2), CONTENT(s2, pos2));
			pos2++;	
			pos1++;
		}
	}

	while(pos1 < size1){
		new = insertElement(new, HASH(s1,pos1), CONTENT(s1,pos1));
		pos1++;
	}
	
	while(pos2 < size2){
		new = insertElement(new, HASH(s2,pos2), CONTENT(s2,pos2));
		pos2++;
	}

	return new;
}

SET diffDataSets(SET s1, SET s2) {
	SET new;
	int res, pos1, pos2, size1, size2;

	pos1 = pos2 = 0;
	size1 = s1->size;
	size2 = s2->size;

	if (size1 > size2)
		new = initSet(size1, s1->free);
	else
		new = initSet(size2, s2->free);

	while(pos1 < size1 && pos2 < size2){
		res = strcmp(HASH(s1,pos1), HASH(s2, pos2));

		if (res < 0) {
			new = insertElement(new, HASH(s1,pos1), CONTENT(s1,pos1));
			pos1++;
		}else if (res > 0){
			new = insertElement(new, HASH(s2,pos2), CONTENT(s2,pos2));
			pos2++;
		}else{
			pos1++;
			pos2++;
		}
	}

	while(pos1 < size1){
		new = insertElement(new, HASH(s1,pos1), CONTENT(s1,pos1));
		pos1++;
	}
	
	while(pos2 < size2){
		new = insertElement(new, HASH(s2,pos2), CONTENT(s2,pos2));
		pos2++;
	}

	return new;
}

SET intersectSet(SET s1, SET s2) {
	SET new;
	int pos1, pos2, size;

	pos1 = pos2 = 0;
	size = (SIZE(s1) < SIZE(s2)) ? SIZE(s1) : SIZE(s2);
	new = initSet(size, s1->free);

	while(pos1 < SIZE(s1) && pos2 < SIZE(s2)) {
		if (strcmp(HASH(s1,pos1), HASH(s2,pos2)) < 0)
			pos1++;	
		else if (strcmp(HASH(s1,pos1), HASH(s2,pos2)) > 0)
			 pos2++;
		else {
			new = insertElement(new, HASH(s1,pos1), CONTENT(s1,pos1));
			pos1++;
			pos2++;
		}
	}

	return new;
}

void freeSet(SET s) {
	int i;

	if (s) {
			for(i = 0; i < SIZE(s); i++){
				free(HASH(s,i));
				if (s->free) s->free(CONTENT(s,i));
				free(s->list[i]);
			}

		free(s->list);
		free(s);
	}
}

static ELEMENT newElement(char* hash, void* content) {
	ELEMENT new = malloc(sizeof(*new));

	new->hash = malloc(sizeof(char) * strlen(hash) + 1);
	strcpy(new->hash, hash);
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

#define SWAP(i,j) tmp=list[i];list[i]=list[j];list[j]=tmp;
static int partition(SET set, int begin, int end, compare_t comparator, void* arg) {
	ELEMENT* list = set->list;
	void* pivot = list[end]->content;
	int i, lim = begin-1;
	ELEMENT tmp;	
	
	for(i = begin; i < end; i++) {
		if (comparator(list[i]->content, pivot, arg) >= 0){
			lim++;
			SWAP(lim, i);
		}
	}

	swapData(set, lim+1, end);
	return lim+1;
}

static void quicksort(SET set, int begin, int end, compare_t comparator, void* arg) {
	int lim;

	if (begin < end) {
		lim = partition(set, begin, end, comparator, arg);
		
		quicksort(set, begin, lim-1, comparator, arg);
		quicksort(set, lim+1, end, comparator, arg);
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
