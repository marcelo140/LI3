#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include "sales.h"

typedef struct table *HASHTABLE;
typedef struct hash *HASH;

HASHTABLE initHashTable();
HASHTABLE insertHashT(HASHTABLE ht, void *back, SALE s);
void      freeHashTable(HASHTABLE ht);

void*  getParent       (HASHTABLE ht, HASH hash);
double getHashBilled   (HASHTABLE ht, HASH hash, int month, int branch, int MODE);
int    getHashQuantity (HASHTABLE ht, HASH hash, int month, int branch, int MODE);
REVENUE getHashRevenue (HASHTABLE ht, HASH hash);

HASH createHash (HASHTABLE ht, char *str);
void freeHash   (HASH hash);

#endif
