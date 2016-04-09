#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include "sales.h"

typedef struct table *HASHTABLE;
typedef struct hash *HASH;

HASHTABLE initHashTable();
HASHTABLE insertHashT(HASHTABLE ht, void *back, SALE s);

HASH createHash(HASHTABLE ht, char *str);

#endif
