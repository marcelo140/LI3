#include <stdlib.h>
#include <string.h>

#include "hashT.h"
#include "set.h"

#define KEY_SIZE 10
#define BASE_CAPACITY 512
#define EMPTY   0
#define BUSY    1
#define REMOVED 2

#define HASH_CRAWLER(i) ht->table[i].status != EMPTY && (ht->table[i].status == REMOVED || strcmp(ht->table[i].key, key)) != 0
#define CAPACITY        ht->capacity
#define KEY(i)     		ht->table[i].key
#define CONTENT(i) 		ht->table[i].content
#define STATUS(i)  		ht->table[i].status

typedef struct hashCntt {
	char key[KEY_SIZE];
	void *content;
	int status;
} HASHTCNTT;

struct hasht {
	struct hashCntt* table;
	int size;
	int maxSize;
	int capacity;

	init_t init;
	add_t add;
	clone_t clone;
	free_t free;
};

static int Hash(char *key);
static HASHT resizeHashT(HASHT ht);

HASHT initMyHashT(int size, init_t init, add_t add, clone_t clone, free_t free) {
	HASHT new = malloc(sizeof(*new));

	new->table    = calloc(size, sizeof(HASHTCNTT));
	new->capacity = size;
	new->size 	  = 0;
	new->maxSize  = new->capacity * 0.8;
	new->init 	  = init;
	new->add  	  = add;
	new->clone    = clone;
	new->free 	  = free;

	return new;
}

HASHT initHashT(init_t init, add_t add, clone_t clone, free_t free) {
	HASHT new = malloc(sizeof(*new));

	new->table    = calloc(BASE_CAPACITY, sizeof(HASHTCNTT));
	new->capacity = BASE_CAPACITY;
	new->size 	  = 0;
	new->maxSize  = new->capacity * 0.8;
	new->init 	  = init;
	new->add  	  = add;
	new->clone    = clone;
	new->free 	  = free;

	return new;
}

HASHT insertHashT(HASHT ht, char* key, void* content) {
	int i, hash, p;
	
	p = hash = Hash(key) & (CAPACITY - 1);
	
	if (ht->size + 1 >= ht->maxSize) ht = resizeHashT(ht);

	for (i=0; i < CAPACITY && HASH_CRAWLER(p); i++)
		p = (hash + (i*i)) & (CAPACITY - 1);

	if (STATUS(p) != BUSY) {
		STATUS(p) = BUSY;
		strncpy(KEY(p), key, KEY_SIZE);
		if (ht->init)
			CONTENT(p) = ht->init();
		ht->size++;
	}

	if(ht->add)
		CONTENT(p) = ht->add(CONTENT(p), content);

	return ht;
}

SET dumpHashT(HASHT ht, SET set) {
	void *contCopy;
	int i;

	for (i=0; i < CAPACITY; i++) {
		if (STATUS(i) == BUSY && CONTENT(i)){
			contCopy = ht->clone(CONTENT(i));
			set = insertElement(set, KEY(i), contCopy);
		}
	}

	return set;
}

void freeHashT(HASHT ht) {
	int i;

	if (!ht) return;

	if (ht->free){
		for(i=0; i < ht->capacity; i++)
			if (STATUS(i) == BUSY) 
				ht->free(CONTENT(i));
	}

	free(ht->table);
	free(ht);
}

void* getHashTcontent(HASHT ht, char* key) {
	int i, p, hash;

	p = hash = Hash(key) & (CAPACITY - 1);

	for(i=0; i < CAPACITY && HASH_CRAWLER(p) ; i++)
		p = (hash + (i*i)) & (CAPACITY - 1);

	return (i < CAPACITY && STATUS(p) == BUSY) ? CONTENT(p) : NULL; 
}

static int Hash(char *key) {
	int i, hash = 5381;
	char c;

	for(i=0; (c = key[i]) ; i++)
		hash = ((hash << 5) + hash) + c;

	return hash;
}

int getHashTsize(HASHT ht){
	return ht->size;
}

static HASHT resizeHashT(HASHT ht){
	HASHT new = malloc(sizeof(*new));
	int i;

	new->capacity = ht->capacity *2;
	new->size 	  = ht->size;
	new->maxSize  = new->capacity * 0.8;
	new->init     = ht->init;
	new->free     = ht->free;
	new->add 	  = ht->add;
	new->clone    = ht->clone;
	
	new->table    = calloc(new->capacity, sizeof(HASHTCNTT));

	for(i=0; i < ht->capacity; i++)
		if (STATUS(i) == BUSY) 
			insertHashT(new, KEY(i), CONTENT(i));

	freeHashT(ht);
	return new;
}
