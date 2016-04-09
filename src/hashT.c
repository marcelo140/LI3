#include <string.h>
#include <stdlib.h>
#include "hashT.h"
#include "revenue.h"

#define MAX_STR_SIZE 10
#define BASE_CAPACITY 31
#define CONT_REVENUE(i) ht->content[i].revenue 


typedef struct hashCntt {
	HASH hash;
	void *back;
	REVENUE revenue;
} HashCntt;

struct table {
	int	capacity;	
	HashCntt* content;
};

struct hash {
	char *seed;
	int key;
};

HASHTABLE initHashTable() {
	HASHTABLE new = malloc(sizeof (*new));
	int i;

	new->capacity = BASE_CAPACITY;
	new->content = malloc(BASE_CAPACITY * sizeof(struct hashCntt));

	for (i=0; i<BASE_CAPACITY; i++) {
		new->content[i].hash = NULL;
		new->content[i].back = NULL;
		new->content[i].revenue = initRevenue();
	}
	
	return new;
}

HASHTABLE insertHashT(HASHTABLE ht, void *back, SALE s) {
	int quantity, billed, month, mode;
	PRODUCT p;
	HASH hash;

	quantity = getQuant(s);
	billed = getPrice(s) * quantity;
	month = getMonth(s);
	mode = getMode(s);	

	p = getProduct(s);
	hash = createHash(ht, fromProduct(p));
	freeProduct(p);

	if (ht->content[hash->key].hash == NULL) {

		ht->content[hash->key].back = back;
		ht->content[hash->key].hash = hash;
		CONT_REVENUE(hash->key) = updateRevenue(CONT_REVENUE(hash->key), 
											month, mode, billed, quantity);
	}else {
		
	}

	return ht;
}

/**
 * Cria uma HASH de uma string para uma dada HASHTABLE
 * @param ht HASHTABLE exemplo
 * @param seed String que serve como seed
 * @return HASH criada
 */
HASH createHash(HASHTABLE ht, char *seed) {
	HASH new = malloc(sizeof(*new));
	int i;
	
	new->seed= malloc(MAX_STR_SIZE);
	strcpy (new->seed, seed);

	/* Este algoritmo de criação de hashes deve ser melhorado */
	for (i=0; seed[i] != '\0' && i < MAX_STR_SIZE; i++) {
		new->key += seed[i];
	} 

	new->key %= ht->capacity;

	return new;
}
