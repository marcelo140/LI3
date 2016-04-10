#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#define MODE_N 0
#define MODE_P 1

#include "sales.h"

typedef struct table *HASHTABLE;
typedef struct hash *HASH;

HASHTABLE initHashTable();
HASHTABLE insertHashT(HASHTABLE ht, void *back, SALE s);
void      freeHashTable(HASHTABLE ht);

void*  getParent       (HASHTABLE ht, HASH hash);
double getHashBilled   (HASHTABLE ht, HASH hash, int month, int MODE);
int    getHashQuantity (HASHTABLE ht, HASH hash, int month, int MODE);


HASH createHash (HASHTABLE ht, char *str);
void freeHash   (HASH hash);

#endif
