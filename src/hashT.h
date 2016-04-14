#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include "sales.h"

typedef struct table *HASHTABLE;
typedef struct hash *HASH;

HASHTABLE initHashTable ();
HASHTABLE insertHashT   (HASHTABLE ht, char *seed, int pos);
int       getPosition   (HASHTABLE ht, char *seed);
void      freeHashTable (HASHTABLE ht);
#endif
