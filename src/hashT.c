#include <stdlib.h>
#include <string.h>
#include "hashT.h"

#define KEY_SIZE 10
#define BASE_CAPACITY 64
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
	ht_init_t init;
	ht_add_t add;
	ht_free_t free;
};

struct hashtSet {
	struct hashCntt* set;
 	int size;
	int capacity;
};

static int Hash(char *key);
static HASHT resizeHashT(HASHT ht);
static HASHTSET initHashTSet(int size); 
static HASHTSET insertHashTSet(HASHTSET hts, HASHTCNTT newCntt); 
static HASHTSET resizeHashTSet(HASHTSET hts);

HASHT initHashT(ht_init_t init, ht_add_t add, ht_free_t free) {
	HASHT new = malloc(sizeof(*new));

	new->table    = calloc(BASE_CAPACITY, sizeof(HASHTCNTT));
	new->capacity = BASE_CAPACITY;
	new->size 	  = 0;
	new->maxSize  = new->capacity * 0.8;
	new->init 	  = init;
	new->add  	  = add;
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
		CONTENT(p) = ht->init();
		ht->size++;
	}
	CONTENT(p) = ht->add(CONTENT(p), content);

	return ht;
}

HASHTSET dumpHashT(HASHT ht) {
	HASHTSET r = initHashTSet(ht->size);
	int i;

	for (i=0; i < CAPACITY; i++) 
		if (STATUS(i) == BUSY) r = insertHashTSet(r, ht->table[i]);

	return r;
}

void* getHashTSetContent(HASHTSET hts, int pos) {
	return hts->set[pos].content;
}

char* getHashTSetKey(HASHTSET hts, int pos) {
	char ret[KEY_SIZE];

	strncpy(ret, hts->set[pos].key, KEY_SIZE);

	return hts->set[pos].key;
}

HASHTSET concatHashTSet(HASHTSET h1, HASHTSET h2) {
	int i;

	for(i=0; i < h2->size; i++)
		insertHashTSet(h1, h2->set[i]);

	return h1;
}

HASHTSET sortHashTSet(HASHTSET hts, compare_t comparator) {
	HASHTSET below, above;
	HASHTCNTT pivot;
	int i, pos;

	if (hts->capacity > 0) {
		pos = hts->size;
		below = initHashTSet(pos/2+1);
		above = initHashTSet(pos/2+1);

		pivot = hts->set[pos-1];

		for(i = 0; i < pos-2; i++) {
			if (comparator(pivot.content, hts->set[i].content) > 0)
				insertHashTSet(above, hts->set[i]);
			else 
				insertHashTSet(below, hts->set[i]);
		}

		below = sortHashTSet(below, comparator);
		above = sortHashTSet(above, comparator);

		below = insertHashTSet(below, pivot);
		below = concatHashTSet(below, above);

		return below;
	}
	
	return hts;
}

void freeHashT(HASHT ht) {
	int i;

	for(i=0; i < ht->capacity; i++)
		if (STATUS(i) == BUSY) {
			free(KEY(i));
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

static HASHT resizeHashT(HASHT ht){
	HASHT new = malloc(sizeof(*new));
	int i;

	new->capacity = ht->capacity *2;
	new->size 	  = ht->size;
	new->maxSize  = new->capacity * 0.8;
	new->init     = ht->init;
	new->free     = ht->free;
	new->add 	  = ht->add;
	
	new->table    = calloc(new->capacity, sizeof(HASHTCNTT));

	for(i=0; i < ht->capacity; i++)
		if (STATUS(i) == BUSY) 
			insertHashT(new, KEY(i), CONTENT(i));

	freeHashT(ht);
	return new;
}

/* ***************** FUNÇÕES AUXILIARES DO HASHTSET ************************** */

static HASHTSET initHashTSet(int size) {
	HASHTSET new = malloc(sizeof(*new));
	new->size = 0;
	new->set  = calloc(size, sizeof(HASHTCNTT));
	new->capacity = size;

	return new;
} 

static HASHTSET insertHashTSet(HASHTSET hts, HASHTCNTT newCntt) {
	HASHTCNTT new;
	strcpy(new.key, newCntt.key);
	new.status  = newCntt.status;
	new.content = newCntt.content;
	
	if (hts->size >= hts->capacity) 
		hts = resizeHashTSet(hts);

	hts->set[hts->size] = new;
	hts->size++;

	return hts;
}

static HASHTSET resizeHashTSet(HASHTSET hts) {
	hts->capacity *= 2;
	hts->set = realloc(hts->set, hts->capacity * sizeof(struct hashCntt) );

	return hts;
}
