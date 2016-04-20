#ifndef __SET__
#define __SET__

#define HASH_SIZE 10

#include "generic.h"

typedef struct element* ELEMENT;
typedef struct set *SET;
typedef struct list *LIST;

char* getListElement(LIST l, int pos);

int getListSize(LIST l);

LIST toList(SET s);

SET initSet(int capacity);

SET insertElement(SET list, char* hash, void* content);

void sortSetByName(SET list);

void sortSet(SET list, compare_t comparator, void* arg);

SET datacpy(SET dest, SET src, int pos);

SET unionSets(SET s1, SET s2);

SET diffSets(SET s1, SET s2);

SET intersectSets(SET s1, SET s2);

char* getSetHash(SET s, int pos);

void* getSetData(SET s, int pos);

int getSetSize(SET s);

void freeSet(SET s);

#endif
