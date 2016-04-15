#include <stdlib.h>
#include <string.h>
#include "hashT.h"

#define BASE_CAPACITY 64
#define EMPTY   0
#define BUSY    1
#define REMOVED 2

#define HASH_CRAWLER(i) i < ht->capacity && ht->table[i].status != EMPTY && strcmp(ht->table[i].key, key) != 0
#define CAPACITY        ht->capacity
#define KEY(i)     		ht->table[i].key
#define CONTENT(i) 		ht->table[i].content
#define STATUS(i)  		ht->table[i].status

struct hashCntt {
	char *key;
	void *content;
	int status;
};

struct hasht {
	struct hashCntt* table;
	int capacity;
	ht_init_t init;
	ht_add_t add;
	ht_free_t free;
};

static int Hash(char *key);
static HASHT resizeHashT(HASHT ht);

HASHT initHashT(ht_init_t init, ht_add_t add, ht_free_t free) {
	HASHT new = malloc(sizeof(*new));

	new->table = calloc(BASE_CAPACITY, sizeof(struct hashCntt));
	new->capacity = BASE_CAPACITY;
	new->init = init;
	new->add  = add;
	new->free = free;

	return new;
}

HASHT insertHashT(HASHT ht, char* key, void* content) {
	int i, hash, p;
	
	p = hash = Hash(key) & (CAPACITY - 1);

	for (i=0; HASH_CRAWLER(p); i++)
		p = hash + (i*i) & (CAPACITY - 1);

	if (i == CAPACITY) {
		ht = resizeHashT(ht);
		ht = insertHashT(ht, key, content);
	} else {
		if (STATUS(p) != BUSY) {
			STATUS(p) = BUSY;
			strcpy(KEY(p), key);
			CONTENT(p) = ht->init();
		}
		CONTENT(p) = ht->add(CONTENT(p), content);
	}

	return ht;
}

static int Hash(char *key) {
	int i, hash = 5381;
	char c;

	for(i=0; (c = key[i]) ; i++)
		hash = (hash << 5) + hash + c;

	return hash;
}

static HASHT resizeHashT(HASHT ht){
	HASHT new = malloc(sizeof(*new));
	int i;

	new->capacity = ht->capacity *2;
	new->init     = ht->init;
	new->free     = ht->free;
	new->add 	  = ht->add;

	for(i=0; i < ht->capacity; i++)
		if (STATUS(i) == BUSY) 
			insertHashT(new, KEY(i), CONTENT(i));

	freeHashT(ht);
	return new;
}
