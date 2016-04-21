#ifndef __SET__
#define __SET__

#include "generic.h"
#define HASH_SIZE 10
typedef struct set *SET;

SET initSet(int capacity);

SET insertElement(SET list, char* hash, void* content);

SET datacpy(SET dest, SET src, int pos);

char* getSetHash(SET s, int pos);

void* getSetData(SET s, int pos);

int getSetSize(SET s);

void sortSetByName(SET list);

void sortSet(SET list, compare_t comparator, void* arg);

SET unionSets(SET s1, SET s2);

SET diffSets(SET s1, SET s2);

SET intersectSet(SET s1, SET s2);

void freeSet(SET s);

#endif
